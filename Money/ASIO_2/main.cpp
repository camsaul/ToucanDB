

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"
#include "TCPConnection.h"
#include "Command.h"
#include "Compression.h"

using namespace toucan_db::logging;

using Clock = chrono::system_clock;

static vector<toucan_db::Client> sClients;

int main(int argc, const char * argv[])
{
	std::ios_base::sync_with_stdio(false);
	
	{
		string input = "get toucan";
		auto cmd = toucan_db::Command::FromInput(input);
		assert(cmd.CommandType() == toucan_db::Command::Type::GET);
		assert(cmd.Key() == "toucan");
		assert(cmd.Val().empty());
		
		auto c = toucan_db::Command::Decode(cmd.Encode());
		assert(c.CommandType() == toucan_db::Command::Type::GET);
		assert(c.Key() == "toucan");
		assert(c.Val().empty());
	}
	
	{
		string input = "set toucan rasta";
		auto cmd = toucan_db::Command::FromInput(input);
		assert(cmd.CommandType() == toucan_db::Command::Type::SET);
		assert(cmd.Key() == "toucan");
		assert(cmd.Val().length() == 5);
		assert(cmd.Val() == "rasta");
		
		auto c = toucan_db::Command::Decode(cmd.Encode());
		assert(c.CommandType() == toucan_db::Command::Type::SET);
		assert(c.Key() == "toucan");
		assert(c.Val() == toucan_db::Compress("rasta"));
	}
	
	toucan_db::server::AsyncServer::Start().Headless(true).NumberOfThreads(std::thread::hardware_concurrency()).Port(1337);
	
	auto client = toucan_db::Client::Create();
	
	using namespace chrono_literals;
	this_thread::sleep_for(300ms);
	
	while (true) {
		cout << RED << "toucan_db> ";
		string input;
		std::getline(std::cin, input);

		try {
			auto command = toucan_db::Command::FromInput(input);
			bool responseWillBeCompressed = command.CommandType() == toucan_db::Command::Type::GET;
			string encoded = command.Encode();
			
			auto start = chrono::system_clock::now();
			string response = client->Request(encoded.data());
			auto end = chrono::system_clock::now() - start;
			
			string decompressedResponse;
			if (responseWillBeCompressed) {
				if (response[0] == '\0') {
					response = "[null]";
				} else {
					response = toucan_db::Decompress(response);
				}
			}
			
			auto ms = chrono::duration_cast<chrono::microseconds>(end).count();
			Logger(BLUE) << "[" << ms << " µs] " << CLEAR_ALL << response;
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