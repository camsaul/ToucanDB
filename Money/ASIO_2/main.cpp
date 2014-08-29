

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"

using namespace toucan_db::logging;

static const size_t kNumIterations = 30000;
static const size_t kNumThreads = 8;
static const size_t kNumRuns = 4;
static const size_t kNumIterationsPerThread = (kNumIterations / kNumRuns) / kNumThreads;
static const size_t kNumServers = 8;

using Clock = chrono::system_clock;

void Set(size_t) {
	Storage::Set("toucan", "rasta");
}

void Get(size_t) {
	bool found;
	Storage::Get("toucan", &found);
}

static vector<toucan_db::Client> sClients;

void DoRequests(size_t i) {
	toucan_db::Client& client = sClients[i % kNumThreads];
	client.Connect(kNumIterationsPerThread);
}

int main(int argc, const char * argv[])
{
	toucan_db::server::AsyncServer::Start().Headless(true).NumberOfThreads(kNumServers);
	
	for (int i = 0; i < kNumThreads; i++) {
		sClients.emplace_back();
	}
	
	{
		Logger(ORANGE) << "Wait for 5 seconds for CPU to cool down...";
	}
	this_thread::sleep_for(chrono::seconds(5));
	
	auto start = Clock::now();
	for (size_t i = 0; i < kNumRuns; i++) {
		tbb::parallel_for(size_t(0), kNumThreads, DoRequests);
	}
	auto time = Clock::now() - start;
	
	auto mcs = chrono::duration_cast<chrono::microseconds>(time);
	auto ms2 = chrono::duration_cast<chrono::milliseconds>(time);
	
	cout << "total time: " << kNumIterations << " requests @ " << double(ms2.count())/1000.0 << " seconds" << endl;
	
	double avgTimePerIteration = mcs.count() / double(kNumIterations);
	cout << "avg time per itr: " << avgTimePerIteration << " microseconds (" << avgTimePerIteration / 1000 << " milliseconds)" << endl;
	
	double itrsPerSecond = chrono::microseconds::period::den / avgTimePerIteration;
	cout << "itrsPerSecond: " << itrsPerSecond << endl;
	
	return 0;
}

//int main(int argc, char* arv[]) {
//	toucan_db::server::AsyncServer::Start();
//	this_thread::sleep_for(chrono::seconds(1));
//	return 0;
//}