

#include <boost/bind.hpp>

#include "Storage.h"

using boost::asio::ip::tcp;

void Server() {
	try {
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1337));
		
		Logger(GREEN) << "Starting server on port 1337...";
		
		while (true) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			
			Logger(GREEN) << "Accepted connection, writing message to +-*9...";
			
			std::string message = "COOL!";
			
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
			
			Logger(GREEN) << "Finished writing message.";
		}
		
	} catch (exception& e) {
		Logger(GREEN, cerr) << e.what();
	}
}

class TCPConnection : public enable_shared_from_this<TCPConnection> {
public:
	static shared_ptr<TCPConnection> Create(boost::asio::io_service& io_service) {
		return shared_ptr<TCPConnection>(new TCPConnection(io_service));
	}
	
	tcp::socket& Socket() { return socket_; }
	
	void Start() {
		static int i = 0;
		message_ = "MY MESSAGE " + lexical_cast<string>(i++);
		
		boost::asio::async_write(socket_, boost::asio::buffer(message_), boost::bind(&TCPConnection::HandleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	
	void HandleWrite(const boost::system::error_code& error, size_t bytesTransferred) {
		// nothing
	}
	
private:
	TCPConnection(boost::asio::io_service& io_service):
		socket_(io_service)
	{}
	
	tcp::socket socket_;
	std::string message_;
};

class AsyncServer {
public:
	AsyncServer(boost::asio::io_service& io_service):
		acceptor_(io_service, tcp::endpoint(tcp::v4(), 1337))
	{
		StartAccept();
	}
	
	void StartAccept() {
		auto newConnection = TCPConnection::Create(acceptor_.get_io_service());
		acceptor_.async_accept(newConnection->Socket(), bind(&AsyncServer::HandleAccept, this, newConnection, boost::asio::placeholders::error));
	}
	
	void HandleAccept(shared_ptr<TCPConnection> newConnection, const boost::system::error_code& error) {
		if (!error) {
			newConnection->Start();
		}
		
		StartAccept();
	}
	
private:
	tcp::acceptor acceptor_;
};

void RunAsyncServer() {
	try {
		Logger(BLUE) << "Starting async server...";
		boost::asio::io_service io_service;
		AsyncServer server(io_service);
		io_service.run();
	} catch (exception& e) {
		Logger(BLUE) << "Caught exception: " << e.what();
	}
}

int main(int argc, char* arv[]) {
	auto t = thread(RunAsyncServer);
	t.detach();
	
	this_thread::sleep_for(chrono::seconds(1));

	
	
	return 0;
}