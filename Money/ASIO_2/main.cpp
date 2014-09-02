

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"
#include "TCPConnection.h"
#include "Command.h"
#include "TString.h"

using namespace toucan_db::logging;

const size_t toucan_db::Client::kNumIterations = 200000;

//static const size_t kNumClients = 16;
static const size_t kNumThreadsPerServer = 2;

// set up separate stacks, clients will use one or the other
static const size_t kNumServers = 1;
static const uint16_t kPorts[] { 1337, 1338, 1339, 1340,
								 1341, 1342, 1343, 1344,
								 1345, 1346, 1347, 1348,
								 1349, 1350, 1351, 1352 };

using Clock = chrono::system_clock;

static vector<toucan_db::Client> sClients;
int PortForI(int i) {
	return kPorts[i % kNumServers];
}

int main(int argc, const char * argv[])
{
	{
		toucan_db::TString s;
		assert(s.IsInline());
		assert(!s);
		s.Hash();
	}
	{
		toucan_db::TString s { "cool" };
		assert(s.IsInline());
		assert(s);
		s.Hash();
	}
	{
		toucan_db::TString s { "really long str" };
		assert(!s.IsInline());
		assert(s);
		s.Hash();
	}
	
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
	
	for (int i = 0; i < kNumServers; i++) {
		toucan_db::server::AsyncServer::Start().Headless(true).NumberOfThreads(kNumThreadsPerServer).Port(PortForI(i));
	}
	
	shared_ptr<toucan_db::Client> client = nullptr;
	thread {[&]{
		client = toucan_db::Client::Create();
	}}.detach();
	while (!client) {
		this_thread::sleep_for(chrono::milliseconds(100));
	}

	while (true) {
		Logger(RED) << "toucan_db> ";
		char input[128];
		cin.getline(input, 128);
		
		try {
			auto command = toucan_db::Command::EncodeInput(input);
			auto start = chrono::system_clock::now();
			auto response = client->Request(command);
			auto end = chrono::system_clock::now() - start;
			auto ms = chrono::duration_cast<chrono::microseconds>(end).count();
			Logger(BLUE) << "[" << ms << " µs] " << response;
			Logger(BLUE) << round(8 * (1000.0 / ms)) << "k requests/sec";
			
		} catch (exception& e) {
			Logger(RED) << e.what();
			continue;
		}
	}
	
	this_thread::sleep_for(chrono::seconds(1));
	return 0;
}

//int main(int argc, char* arv[]) {
//	toucan_db::server::AsyncServer::Start();
//	this_thread::sleep_for(chrono::seconds(1));
//	return 0;
//}