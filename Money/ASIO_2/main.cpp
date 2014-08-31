

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"
#include "TCPConnection.h"

using namespace toucan_db::logging;

const size_t toucan_db::Client::kNumIterations = 10000;

static const size_t kNumClients = 8;
static const size_t kNumRuns = 1;
static const size_t kNumIterationsPerThread = (toucan_db::Client::kNumIterations / kNumRuns) / kNumClients;
static const size_t kNumServers = 4;

// set up separate stacks, clients will use one or the other
static const size_t kNumVirtualStacks = 2;
static const uint16_t kPorts[] { 1337, 1338, 1339, 1340, 1341, 1342, 1343, 1344 };

using Clock = chrono::system_clock;

static vector<toucan_db::Client> sClients;
int PortForI(int i) {
	return kPorts[i % kNumVirtualStacks];
}

int main(int argc, const char * argv[])
{
	toucan_db::SetTheCans();
	
	for (int i = 0; i < kNumVirtualStacks; i++) {
		toucan_db::server::AsyncServer::Start().Headless(true).NumberOfThreads(kNumServers).Port(PortForI(i));
	}
	
	toucan_db::Client::sRequestsCount = 0;
	vector<shared_ptr<toucan_db::Client>> clients;
		
	for (int i = 0; i < kNumClients; i++) {
		auto c = make_shared<toucan_db::Client>("172.20.10.3", PortForI(i));
		clients.push_back(c);
	}
		
	std::vector<std::function<void()>> clientFns;
	for (int i = 0; i < kNumClients; i++) {
	shared_ptr<toucan_db::Client> clientPtr = clients[i];
		clientFns.push_back(std::bind(&toucan_db::Client::Connect, clientPtr));
	}
	
	Logger(ORANGE) << "Wait for 2 seconds for CPU to cool down...";
	this_thread::sleep_for(chrono::seconds(2));
	
	cout << "START" << endl;
	auto start = Clock::now();
	
	for (int i = 0; i < kNumClients; i++) {
		thread(clientFns[i]).detach();
	}
	
	int lastCount = 0;
	int lastReadsCount = 0;
	int lastGets = 0;
	int lastConnectionsDestroyed = 0;
	
	int peakRequests = 0;
	using namespace chrono_literals;
	static const auto kLogInterval = 500ms;
	
	while (true) {
		int totalCount = toucan_db::Client::sRequestsCount;
		int countThisRound = totalCount - lastCount;
		if (countThisRound > peakRequests) peakRequests = countThisRound;
		lastCount = totalCount;
		
		int connections = toucan_db::server::AsyncServer::sNumberOfConnectionsAccepted;
		toucan_db::server::AsyncServer::sNumberOfConnectionsAccepted = 0;
		
		int writes = toucan_db::TCPConnection::sNumWrites;
		toucan_db::TCPConnection::sNumWrites = 0;
		
		int totalReads = toucan_db::Client::sReadsCount;
		int readsThisRound = totalReads - lastReadsCount;
		lastReadsCount = totalReads;
		
		int totalGets = toucan_db::Storage::sNumGets;
		int getsThisRound = totalGets - lastGets;
		lastGets = totalGets;
		
		int totalConnectionsDestroyed = toucan_db::TCPConnection::sNumDestroyed;
		int destroyedThisRound = totalConnectionsDestroyed - lastConnectionsDestroyed;
		lastConnectionsDestroyed = totalConnectionsDestroyed;

		Logger(countThisRound == 0 ? RED : GREEN) << "requests: " << countThisRound << " (" << std::round((100.0 * totalCount) / toucan_db::Client::kNumIterations) << " %)\tconnections accepted: " << connections << "\tdestroyed: " << destroyedThisRound << "\twrites: " << writes << "\treads: " << readsThisRound << "\tgets: " << getsThisRound;
		
		if (totalCount >= toucan_db::Client::kNumIterations) break;
		this_thread::sleep_for(kLogInterval);
	}
	auto time = Clock::now() - start;
	
	auto mcs = chrono::duration_cast<chrono::microseconds>(time);
	auto ms2 = chrono::duration_cast<chrono::milliseconds>(time);
	
	Logger(ORANGE) << "\n------------------------------RESULTS------------------------------------";
	cout << kNumClients << " client threads." << endl;
	cout << kNumVirtualStacks * kNumServers << " server threads (" << kNumVirtualStacks << " ports running " << kNumServers << " threads each)." << endl;
	Logger(ORANGE) << "\n-------------------------------TIME--------------------------------------";
	
	cout << "total time: " << toucan_db::Client::kNumIterations << " requests @ " << double(ms2.count())/1000.0 << " seconds" << endl;
	
	double avgTimePerIteration = mcs.count() / double(toucan_db::Client::kNumIterations);
	cout << "avg time per itr: " << avgTimePerIteration << " microseconds (" << avgTimePerIteration / 1000 << " milliseconds)" << endl;
	
	double itrsPerSecond = chrono::microseconds::period::den / avgTimePerIteration;
	Logger(BLUE) << (int)itrsPerSecond << " avg requests per second.";
	
	auto peakRequestRate = ((float)peakRequests / (chrono::duration_cast<chrono::milliseconds>(1 * kLogInterval).count())) * 1000.0;
	Logger(GREEN) << "Peak request rate: " << peakRequestRate << " requests/sec";
	
	return 0;
}

//int main(int argc, char* arv[]) {
//	toucan_db::server::AsyncServer::Start();
//	this_thread::sleep_for(chrono::seconds(1));
//	return 0;
//}