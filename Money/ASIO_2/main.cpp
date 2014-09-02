

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
		auto encoded = toucan_db::Command::EncodeInput(input).data();
		assert(encoded[0] == 'g');
		toucan_db::Command::KeyLengthT size;
		memcpy(&size, encoded + 1, sizeof(size));
		assert(size == 6);
		
		auto c = toucan_db::Command::Decode(const_cast<char*>(encoded));
		assert(c.CommandType() == toucan_db::Command::Type::GET);
		assert(c.Key());
		assert(string(c.Key()) == "toucan");
	}
	
	{
		char input[] {"set toucan rasta"};
		auto encoded = toucan_db::Command::EncodeInput(input).data();
		assert(encoded[0] == 's');
		toucan_db::Command::KeyLengthT size;
		memcpy(&size, encoded + 1, sizeof(size));
		assert(size == 6);
		
		auto c = toucan_db::Command::Decode(const_cast<char*>(encoded));
		assert(c.CommandType() == toucan_db::Command::Type::SET);
		assert(c.Key());
		assert(string(c.Key()) == "toucan");
		assert(c.Val());
//		assert(string(c.Val()) == "rasta");
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
			const bool isCompressed = command[0] == static_cast<char>(toucan_db::Command::Type::GET);
			if (command.empty()) continue;
			
			auto start = chrono::system_clock::now();
			string response = client->Request(command.data());
			auto end = chrono::system_clock::now() - start;
			
			string decompressedResponse;
			if (isCompressed) {
				google::protobuf::io::ArrayInputStream i(&response[0], response.size());
				auto ci = unique_ptr<zerocc::AbstractCompressedInputStream>(get_compressed_input_stream(&i, zerocc::LZ4));
				{
					google::protobuf::io::CodedInputStream c(ci.get());
					uint32_t size;
					c.ReadVarint32(&size);
					c.ReadString(&decompressedResponse, size);
				}
			} else {
				decompressedResponse = response;
			}
			
			auto ms = chrono::duration_cast<chrono::microseconds>(end).count();
			Logger(BLUE) << "[" << ms << " µs] " << decompressedResponse;
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