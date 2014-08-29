

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"
#include "TCPConnection.h"

using namespace toucan_db::logging;

static const size_t kNumIterations = 10000;
static const size_t kNumClients = 8;
static const size_t kNumRuns = 1;
static const size_t kNumIterationsPerThread = (kNumIterations / kNumRuns) / kNumClients;
static const size_t kNumServers = 16;

// set up separate stacks, clients will use one or the other
static const size_t kNumVirtualStacks = 1;
static const uint16_t kPorts[] { 1337, 1338, 1339, 1340, 1341, 1342, 1343, 1344 };

using Clock = chrono::system_clock;

static vector<toucan_db::Client> sClients;

void DoRequests(size_t i) {
	toucan_db::Client& client = sClients[i % kNumClients];
	client.Connect(kNumIterationsPerThread);
}

int main(int argc, const char * argv[])
{
	toucan_db::SetTheCans();
	
	for (int i = 0; i < kNumVirtualStacks; i++) {
		toucan_db::server::AsyncServer::Start().Headless(true).NumberOfThreads(kNumServers).Port(kPorts[i]);
	}
	
	for (int i = 0; i < kNumClients; i++) {
		sClients.emplace_back("172.20.10.3", kPorts[i % kNumVirtualStacks]);
	}
	
	{
		Logger(ORANGE) << "Wait for 2 seconds for CPU to cool down...";
	}
	this_thread::sleep_for(chrono::seconds(2));
	
	cout << "START" << endl;
	auto start = Clock::now();
	for (size_t i = 0; i < kNumRuns; i++) {
		tbb::parallel_for(size_t(0), kNumClients, DoRequests);
	}
	auto time = Clock::now() - start;
	
	auto mcs = chrono::duration_cast<chrono::microseconds>(time);
	auto ms2 = chrono::duration_cast<chrono::milliseconds>(time);
	
	Logger(ORANGE) << "\n------------------------------RESULTS------------------------------------";
	cout << kNumClients << " client threads." << endl;
	cout << kNumVirtualStacks * kNumServers << " server threads (" << kNumVirtualStacks << " ports running " << kNumServers << " threads each)." << endl;
	Logger(ORANGE) << "\n-------------------------------TIME--------------------------------------";
	
	cout << "total time: " << kNumIterations << " requests @ " << double(ms2.count())/1000.0 << " seconds" << endl;
	
	double avgTimePerIteration = mcs.count() / double(kNumIterations);
	cout << "avg time per itr: " << avgTimePerIteration << " microseconds (" << avgTimePerIteration / 1000 << " milliseconds)" << endl;
	
	double itrsPerSecond = chrono::microseconds::period::den / avgTimePerIteration;
	Logger(BLUE) << (int)itrsPerSecond << " requests per second.";
	
	return 0;
}

//int main(int argc, char* arv[]) {
//	toucan_db::server::AsyncServer::Start();
//	this_thread::sleep_for(chrono::seconds(1));
//	return 0;
//}