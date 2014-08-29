#include "http_connection.h"

namespace crow {
	Connection::Connection(boost::asio::io_service& io_service, Handler* handler):
		socket_	  (io_service),
		handler_  (handler),
		parser_	  (this)
	{
		cout << "new Connection(): " << hex << this << dec << endl;
//		io_service.run();
	}
	
	Connection::~Connection() {
		cout << "~Connection(): " << hex << this << dec << endl;
//		cout << '~' << endl;
		res.complete_request_handler_ = nullptr;
		cancel_deadline_timer();
	}
	
	void Connection::start() {
		cout << "\n--------------------" << endl;
		cout << "Connection::start(): " << hex << this << dec << endl;
		start_deadline();
		
		do_read();
	}
	
	void Connection::HandleExpect100Continue() {
		cout << "Connection::HandleExpect100Continue(): " << hex << this << dec << endl;
	
		buffers_.clear();
		static const char * const kExpect100Continue = "HTTP/1.1 100 Continue\r\n\r\n";
		static const size_t kExpect100ContinueLength = strlen(kExpect100Continue);
		
		buffers_.emplace_back(kExpect100Continue, kExpect100ContinueLength);
		close_connection_ = true;
		do_write();
	}
	
	void Connection::handle()
	{
		cout << "Connection::handle(): " << hex << this << dec << endl;
		cancel_deadline_timer();
		
		request req = parser_.to_request();
		close_connection_ = true;

		res.complete_request_handler_ = [this]{ this->complete_request(); };
		res.is_alive_helper_ = [this]()->bool{ return socket_.is_open(); };
		handler_->Handle(req, res);
	}
	
	void Connection::complete_request()
	{
		cout << "Connection::complete_request(): " << hex << this << dec << endl;
		res.complete_request_handler_ = nullptr;
		
		if (!socket_.is_open()) return;

		static const map<int, const char *> kStatusCodes {
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
		static const char * const kCRLFs = "\r\n\r\n";
		static const size_t kCRLFsLength = strlen(kCRLFs);
		
		buffers_.clear();
		buffers_.reserve(4*(res.headers.size()+4)+3);
		
		const auto itr = kStatusCodes.find(res.code);
		const char* message = nullptr;
		if (itr == kStatusCodes.end()) {
			res.code = 500;
			message = kStatusCodes.find(500)->second;
			
		} else {
			message = itr->second;
			buffers_.emplace_back(message, strlen(message));
		}
		
		if (res.code >= 400 && res.body_.empty()) {
			res.body_ = message[9];
		}
		
		static const char * const kContentLengthHeader = "Content-Length: ";
		static const size_t kContentLengthHeaderLength = strlen(kContentLengthHeader);
		const auto contentLength = lexical_cast<string>(res.body_.size());
		
		buffers_.emplace_back(kContentLengthHeader, kContentLengthHeaderLength);
		buffers_.emplace_back(contentLength.data(), contentLength.size());
		buffers_.emplace_back(kCRLFs, kCRLFsLength);
		buffers_.emplace_back(res.body_.data(), res.body_.size());
		
		close_connection_ = true;
		do_write();
		res.clear();
		
		cout << "socket_.close()" << endl;
		socket_.close();
	}
	
	void Connection::do_read()
	{
		cout << "Connection::do_read(): " << hex << this << dec << endl;
		
		is_reading = true;
		boost::system::error_code ec;
		ec.clear();
		
		while (!ec) {
			std::vector<boost::asio::mutable_buffers_1> bufs;
//			array<boost::asio::buffer, bufs>;
//			auto buf = boost::asio::buffer();
			socket_.read_some(bufs, ec);
			for (auto& buf : bufs) {
				bool ret = parser_.feed(buf.data(), buf.size());
				if (!ret || !socket_.is_open() || close_connection_) break;
			}
		}
		parser_.done();
		socket_.close();
		check_destroy();
		
//		while (!ec)
//		socket_.async_read_some(boost::asio::buffer(buffer_),
//			[this](const boost::system::error_code& ec, std::size_t bytes_transferred)
//			{
//				cout << "Connection::async_read_some callback.: " << hex << this << dec << endl;
//
//				bool error_while_reading = true;
//				if (!ec)
//				{
//					bool ret = parser_.feed(buffer_.data(), bytes_transferred);
//					if (ret && socket_.is_open() && !close_connection_)
//					{
//						error_while_reading = false;
//					}
//				}
//				
//				if (error_while_reading)
//				{
//					cancel_deadline_timer();
//					parser_.done();
//					cout << "socket_.close()" << endl;
//					socket_.close();
//					is_reading = false;
//					check_destroy();
//				}
//				else
//				{
//					do_read();
//				}
//			});
	}
	
	void Connection::do_write()
	{
		cout << "Connection::do_write(): " << hex << this << dec << endl;
		
		boost::asio::write(socket_, buffers_);
		cout << "socket_.close()" << endl;
		socket_.close();
		check_destroy();
		
//		is_writing = true;
//		boost::asio::async_write(socket_, buffers_,
//			 [&](const boost::system::error_code& ec, std::size_t bytes_transferred)
//			 {
//				 cout << "Connection::async_write callback.: " << hex << this << dec << endl;
//				 is_writing = false;
//				 if (!ec)
//				 {
//					 if (close_connection_)
//					 {
//						 cout << "socket_.close()" << endl;
//						 socket_.close();
//						 check_destroy();
//					 }
//				 }
//				 else
//				 {
//					 check_destroy();
//				 }
//			 });
	}
	
	void Connection::check_destroy()
	{
		cout << "Connection::check_destroy()" << endl;
		if (!is_reading && !is_writing)
		{
			res.complete_request_handler_ = nullptr;
			cancel_deadline_timer();
			delete this;
		}
	}
	
//	void Connection::cancel_deadline_timer()
//	{
//		cout << "Connection::cancel_deadline_timer(): " << hex << this << dec << endl;
//		timer_cancel_key_.Cancel();
//	}
	
//	void Connection::start_deadline(int timeout)
//	{
//		auto& timer_queue = detail::DumbTimerQueue::Current();
//		cancel_deadline_timer();
//		
//		timer_cancel_key_ = timer_queue.Add([this]{
//			if (!socket_.is_open())
//			{
//				return;
//			}
//			socket_.close();
//		});
//	}
}