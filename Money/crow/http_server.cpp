#include "http_server.h"

namespace crow {
	Server::Server(Handler* handler, uint16_t port, uint16_t concurrency):
		acceptor_	(io_service_, tcp::endpoint(asio::ip::address(), port)),
		signals_	(io_service_, SIGINT, SIGTERM),
		handler_	(handler),
		concurrency_(concurrency),
		port_		(port)
	{}
	
	void Server::run()
	{
		assert(concurrency_ > 0);
		
		for(int i = 0; i < concurrency_;  i++) {
			io_service_pool_.emplace_back(new boost::asio::io_service());
		}
		
		vector<future<void>> v;
		for(uint16_t i = 0; i < concurrency_; i ++)
			v.push_back(
				async(launch::async, [this, i]{
					auto& timer_queue = detail::dumb_timer_queue::get_current_dumb_timer_queue();
					timer_queue.set_io_service(*io_service_pool_[i]);
					boost::asio::deadline_timer timer(*io_service_pool_[i]);
					timer.expires_from_now(boost::posix_time::seconds(1));
					std::function<void(const boost::system::error_code& ec)> handler;
					handler = [&](const boost::system::error_code& ec){
						if (ec)
							return;
						timer_queue.process();
						timer.expires_from_now(boost::posix_time::seconds(1));
						timer.async_wait(handler);
					};
					timer.async_wait(handler);
					io_service_pool_[i]->run();
				}
			));
		CROW_LOG_INFO << "Server is running, local port " << port_;
		
		signals_.async_wait(
			[&](const boost::system::error_code& error, int signal_number){
				stop();
			});
		
		do_accept();
		
		v.push_back(async(launch::async, [this]{
			io_service_.run();
			CROW_LOG_INFO << "Exiting.";
		}));
	}
	
	void Server::stop()
	{
		io_service_.stop();
		for(auto& io_service:io_service_pool_) {
			io_service->stop();
		}
	}
	
	asio::io_service& Server::pick_io_service()
	{
		// TODO load balancing
		roundrobin_index_++;
		if (roundrobin_index_ >= io_service_pool_.size())
			roundrobin_index_ = 0;
		cout << "INDEX -> " << roundrobin_index_ << endl;
		return *io_service_pool_[roundrobin_index_];
	}
	
	void Server::do_accept()
	{
		auto p = new Connection(pick_io_service(), handler_);
		acceptor_.async_accept(p->socket(),
		   [this, p](boost::system::error_code ec)
		   {
			   if (!ec)
			   {
				   p->start();
			   }
			   do_accept();
		   });
	}
}