

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"
#include "TCPConnection.h"
#include "Command.h"
#include "Compression.h"

#include "BitFlipUtils.h"
#include "TaggedPtr.h"
#include "Timed.h"
#include "TString.h"

using namespace toucan_db;
using namespace toucan_db::logging;
using namespace toucan_db::bit_flip;

using Clock = chrono::system_clock;

static vector<toucan_db::Client> sClients;


int main(int argc, const char * argv[])
{
	std::ios_base::sync_with_stdio(false);
	
	static const size_t k10Million = 10_p * 1000_p * 1000_p;
	for (int strSize = 2; strSize < 100000; strSize *= 10) {
		char str[strSize];
		auto range = 'z' - 'a';
		for (int i = 0; i < strSize - 1; i++) {
			str[i] = (i % range) + 'a';
		}
		str[strSize - 1] = 0;
		
		Logger(RED) << "---------------------- len = " << strSize << " ----------------------";
		
		Logger(RED) << "char*";
		Timed([s = (char *)str]{
			int len = 0;
			for (size_t i = 0; i < k10Million; i++) {
				len += strlen(s);
			}
			ostringstream os;
			os << s;
			Logger(ORANGE) << len;
		});
		
		Logger(RED) << "std::string";
		Timed([cp = (char *)str]{
			string s = cp;
			int len = 0;
			for (size_t i = 0; i < k10Million; i++) {
				len += s.length();
			}
			ostringstream os;
			os << s;
			Logger(ORANGE) << len;
		});
		
		Logger(RED) << "TString";
		Timed([cp = (char *)str]{
			TString s { cp };
			int len = 0;
			for (size_t i = 0; i < k10Million; i++) {
				len += s.Length();
			}
			ostringstream os;
			os << s;
			Logger(ORANGE) << len;
		});
	}

	// strcmp test
	
	exit(0);
	

	cout << BLUE;
	for (int i = 63; i >= 0; i-= 8) {
		cout << setw(9) << left << dec << i;
	}
	cout << endl;
	
	for (int i = 0; i < 300; i += 3) {
		cout << DumpBinary<size_t>(i) << endl;
	}
	
	srand(time(NULL));
	for (int i = 1; i < 100; i++) {
		auto ptr = UniqueCPtr<char>(rand() % 1024);
		assert(ptr);
		cout << ptr << endl;
	}
	
	auto ptr = TaggedPtr(1);
	assert(ptr.GetLower() == 1);
	assert(TaggedPtr(2).GetLower() == 2);
	assert(TaggedPtr(15).GetLower() == 7);
	assert(TaggedPtr(15).GetUntagged() == 8);
	cout << DumpBinary<size_t>(TaggedPtr(9999).GetTagged()) << endl;
	
	
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