#include "http_connection.h"

namespace crow {
	Connection::Connection(boost::asio::io_service& io_service, Handler* handler):
		socket_(io_service),
		handler_(handler),
		parser_(this)
	{
		#ifdef CROW_ENABLE_DEBUG
			connectionCount ++;
			CROW_LOG_DEBUG << "Connection open, total " << connectionCount << ", " << this;
		#endif
	}
	
	Connection::~Connection() {
		res.complete_request_handler_ = nullptr;
		cancel_deadline_timer();
		#ifdef CROW_ENABLE_DEBUG
			connectionCount --;
			CROW_LOG_DEBUG << "Connection closed, total " << connectionCount << ", " << this;
		#endif
	}
	
	void Connection::start() {
		//auto self = this->shared_from_this();
		start_deadline();
		
		do_read();
	}
	
	void Connection::handle_header() {
		// HTTP 1.1 Expect: 100-continue
		if (parser_.check_version(1, 1) && parser_.headers.count("expect") && parser_.headers["expect"] == "100-continue")
		{
			buffers_.clear();
			static std::string expect_100_continue = "HTTP/1.1 100 Continue\r\n\r\n";
			buffers_.emplace_back(expect_100_continue.data(), expect_100_continue.size());
			do_write();
		}
	}
	
	void Connection::handle()
	{
		cancel_deadline_timer();
		bool is_invalid_request = false;
		
		request req = parser_.to_request();
		close_connection_ = true;
		
//		if (parser_.check_version(1, 0))
//		{
//			// HTTP/1.0
//			if (!(req.headers.count("connection") && boost::iequals(req.headers["connection"],"Keep-Alive")))
//				close_connection_ = true;
//		}
//		else if (parser_.check_version(1, 1))
//		{
//			// HTTP/1.1
//			if (req.headers.count("connection") && req.headers["connection"] == "close")
//				close_connection_ = true;
//			if (!req.headers.count("host"))
//			{
//				is_invalid_request = true;
//				res = response(400);
//			}
//		}
		
		CROW_LOG_INFO << "Request: " << boost::lexical_cast<std::string>(socket_.remote_endpoint()) << " " << this
			<< " HTTP/" << parser_.http_major << "." << parser_.http_minor << ' ' << method_name(req.method) << " " << req.url;
		
		
		if (!is_invalid_request)
		{
			res.complete_request_handler_ = [this]{ this->complete_request(); };
			res.is_alive_helper_ = [this]()->bool{ return socket_.is_open(); };
			handler_->Handle(req, res);
		}
		else
		{
			complete_request();
		}
	}
	
	void Connection::complete_request()
	{
		CROW_LOG_INFO << "Response: " << this << ' ' << res.code << ' ' << close_connection_;
		
		//auto self = this->shared_from_this();
		res.complete_request_handler_ = nullptr;
		
		if (!socket_.is_open()) return;

		static std::unordered_map<int, std::string> statusCodes = {
			{200, "HTTP/1.1 200 OK\r\n"},
			{201, "HTTP/1.1 201 Created\r\n"},
			{202, "HTTP/1.1 202 Accepted\r\n"},
			{204, "HTTP/1.1 204 No Content\r\n"},
			
			{300, "HTTP/1.1 300 Multiple Choices\r\n"},
			{301, "HTTP/1.1 301 Moved Permanently\r\n"},
			{302, "HTTP/1.1 302 Moved Temporarily\r\n"},
			{304, "HTTP/1.1 304 Not Modified\r\n"},
			
			{400, "HTTP/1.1 400 Bad Request\r\n"},
			{401, "HTTP/1.1 401 Unauthorized\r\n"},
			{403, "HTTP/1.1 403 Forbidden\r\n"},
			{404, "HTTP/1.1 404 Not Found\r\n"},
			
			{500, "HTTP/1.1 500 Internal Server Error\r\n"},
			{501, "HTTP/1.1 501 Not Implemented\r\n"},
			{502, "HTTP/1.1 502 Bad Gateway\r\n"},
			{503, "HTTP/1.1 503 Service Unavailable\r\n"},
		};
		
		static std::string seperator = ": ";
		static std::string crlf = "\r\n";
		
		buffers_.clear();
		buffers_.reserve(4*(res.headers.size()+4)+3);
		
//		if (res.body.empty() && res.json_value.t() == json::type::Object)
//		{
//			res.body = json::dump(res.json_value);
//		}
		
		if (!statusCodes.count(res.code))
			res.code = 500;
		{
			auto& status = statusCodes.find(res.code)->second;
			buffers_.emplace_back(status.data(), status.size());
		}
		
		if (res.code >= 400 && res.body_.empty())
			res.body_ = statusCodes[res.code].substr(9);
		
		bool has_content_length = false;
		
		for(auto& kv : res.headers)
		{
			buffers_.emplace_back(kv.first.data(), kv.first.size());
			buffers_.emplace_back(seperator.data(), seperator.size());
			buffers_.emplace_back(kv.second.data(), kv.second.size());
			buffers_.emplace_back(crlf.data(), crlf.size());
			
			if (boost::iequals(kv.first, "content-length"))
				has_content_length = true;
		}
		
		if (!has_content_length)
		{
			content_length_ = std::to_string(res.body_.size());
			static std::string content_length_tag = "Content-Length: ";
			buffers_.emplace_back(content_length_tag.data(), content_length_tag.size());
			buffers_.emplace_back(content_length_.data(), content_length_.size());
			buffers_.emplace_back(crlf.data(), crlf.size());
		}
		
		buffers_.emplace_back(crlf.data(), crlf.size());
		buffers_.emplace_back(res.body_.data(), res.body_.size());
		
		close_connection_ = true;
		do_write();
		res.clear();
	}
	
	void Connection::do_read()
	{
		//auto self = this->shared_from_this();
		is_reading = true;
		socket_.async_read_some(boost::asio::buffer(buffer_),
			[this](const boost::system::error_code& ec, std::size_t bytes_transferred)
			{
				bool error_while_reading = true;
				if (!ec)
				{
					bool ret = parser_.feed(buffer_.data(), bytes_transferred);
					if (ret && socket_.is_open() && !close_connection_)
					{
						error_while_reading = false;
					}
				}
				
				if (error_while_reading)
				{
					cancel_deadline_timer();
					parser_.done();
					socket_.close();
					is_reading = false;
					CROW_LOG_DEBUG << this << " from read(1)";
					check_destroy();
				}
				else
				{
					start_deadline();
					do_read();
				}
			});
	}
	
	void Connection::do_write()
	{
		//auto self = this->shared_from_this();
		is_writing = true;
		boost::asio::async_write(socket_, buffers_,
			 [&](const boost::system::error_code& ec, std::size_t bytes_transferred)
			 {
				 is_writing = false;
				 if (!ec)
				 {
					 if (close_connection_)
					 {
						 socket_.close();
						 CROW_LOG_DEBUG << this << " from write(1)";
						 check_destroy();
					 }
				 }
				 else
				 {
					 CROW_LOG_DEBUG << this << " from write(2)";
					 check_destroy();
				 }
			 });
	}
	
	void Connection::check_destroy()
	{
		CROW_LOG_DEBUG << this << " is_reading " << is_reading << " is_writing " << is_writing;
		if (!is_reading && !is_writing)
		{
			CROW_LOG_DEBUG << this << " delete (idle) ";
			delete this;
		}
	}
	
	void Connection::cancel_deadline_timer()
	{
		CROW_LOG_DEBUG << this << " timer cancelled: " << timer_cancel_key_.first << ' ' << timer_cancel_key_.second;
		detail::dumb_timer_queue::get_current_dumb_timer_queue().cancel(timer_cancel_key_);
	}
	
	void Connection::start_deadline(int timeout)
	{
		auto& timer_queue = detail::dumb_timer_queue::get_current_dumb_timer_queue();
		cancel_deadline_timer();
		
		timer_cancel_key_ = timer_queue.add([this]{
			if (!socket_.is_open())
			{
				return;
			}
			socket_.close();
		});
		CROW_LOG_DEBUG << this << " timer added: " << timer_cancel_key_.first << ' ' << timer_cancel_key_.second;
	}

}