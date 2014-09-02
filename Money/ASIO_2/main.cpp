

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"
#include "TCPConnection.h"
#include "Command.h"

using namespace toucan_db::logging;

using Clock = chrono::system_clock;

static vector<toucan_db::Client> sClients;

int main(int argc, const char * argv[])
{
	{
		char input[] {"get toucan"};
		char* encoded = toucan_db::Command::EncodeInput(input);
		assert(encoded[0] == 'g');
		toucan_db::Command::KeyLengthT size;
		memcpy(&size, encoded + 1, sizeof(size));
		assert(size == 6);
		
		auto c = toucan_db::Command::Decode(encoded);
		assert(c.CommandType() == toucan_db::Command::Type::GET);
		assert(c.Key());
		assert(string(c.Key()) == "toucan");
	}
	
	{
		char input[] {"set toucan rasta"};
		char* encoded = toucan_db::Command::EncodeInput(input);
		assert(encoded[0] == 's');
		toucan_db::Command::KeyLengthT size;
		memcpy(&size, encoded + 1, sizeof(size));
		assert(size == 6);
		
		auto c = toucan_db::Command::Decode(encoded);
		assert(c.CommandType() == toucan_db::Command::Type::SET);
		assert(c.Key());
		assert(string(c.Key()) == "toucan");
		assert(c.Val());
		assert(string(c.Val()) == "rasta");
	}
	
	toucan_db::server::AsyncServer::Start().Headless(true).NumberOfThreads(std::thread::hardware_concurrency()).Port(1337);
	
	shared_ptr<toucan_db::Client> client = nullptr;
	thread {[&]{
		client = toucan_db::Client::Create();
	}}.detach();
	while (!client) {
		this_thread::sleep_for(chrono::milliseconds(100));
	}

	while (true) {
		cout << RED << "toucan_db> ";
		char input[128];
		cin.getline(input, 128);
		
		try {
			auto command = toucan_db::Command::EncodeInput(input);
			if (!command) continue;
			
			auto start = chrono::system_clock::now();
			auto response = client->Request(command);
			auto end = chrono::system_clock::now() - start;
			
			auto ms = chrono::duration_cast<chrono::microseconds>(end).count();
			Logger(BLUE) << "[" << ms << " µs] " << response;
			Logger(BLUE) << round(std::thread::hardware_concurrency() * (1000.0 / ms)) << "k requests/sec";
			
		} catch (exception& e) {
			Logger(RED) << e.what();
			continue;
		}
	}
	
	toucan_db::server::AsyncServer::StopAll();
	
	return 0;
}

//int main(int argc, char* arv[]) {
//	toucan_db::server::AsyncServer::Start();
//	this_thread::sleep_for(chrono::seconds(1));
//	return 0;
//}