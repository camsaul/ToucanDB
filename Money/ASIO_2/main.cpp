

#include "AsyncServer.h"
#include "Storage.h"
#include "Client.h"
#include "Logging.h"
#include "TCPConnection.h"

using namespace toucan_db::logging;

const size_t toucan_db::Client::kNumIterations = 75000;

static const size_t kNumClients = 32;
static const size_t kNumThreadsPerServer = 32;

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
	toucan_db::SetTheCans();
	
	{
		for (int i = 0; i < kNumServers; i++) {
			toucan_db::server::AsyncServer::Start().Headless(true).NumberOfThreads(kNumThreadsPerServer).Port(PortForI(i));
		}
		
		toucan_db::Client::sRequestsCount = 0;
		vector<shared_ptr<toucan_db::Client>> clients;
		
		for (int i = 0; i < kNumClients; i++) {
			clients.push_back(toucan_db::Client::Create("172.20.10.3", PortForI(i)));
		}
		
		std::vector<std::function<void()>> clientFns;
		for (int i = 0; i < kNumClients; i++) {
			auto connectFn = [clientPtr = clients[i]]{
				clientPtr->Connect();
			};
			clientFns.push_back(connectFn);
		}
		
		using namespace chrono_literals;
		static const auto kLogInterval = 500ms;
		
		Logger(ORANGE) << "Wait for 1 second for server to start...";
		this_thread::sleep_for(chrono::seconds(1));
		
		Logger(ORANGE) << "Starting clock...";
		auto start = Clock::now();
		
		for (int i = 0; i < kNumClients; i++) {
			thread(clientFns[i]).detach();
		}
		
		int lastCount = 0;
		int peakRequests = 0;
		
		while (true) {
			int totalCount = toucan_db::Client::sRequestsCount;
			int countThisRound = totalCount - lastCount;
			if (countThisRound > peakRequests) peakRequests = countThisRound;
			lastCount = totalCount;
			
			Logger(countThisRound == 0 ? RED : GREEN) << "requests: " << countThisRound << " (" << std::round((100.0 * totalCount) / toucan_db::Client::kNumIterations) << " %)";
			
			if (totalCount >= toucan_db::Client::kNumIterations) break;
			this_thread::sleep_for(kLogInterval);
		}
		
		auto time = Clock::now() - start;
		
		auto mcs = chrono::duration_cast<chrono::microseconds>(time);
		auto ms2 = chrono::duration_cast<chrono::milliseconds>(time);
		
		Logger(ORANGE) << "\n------------------------------RESULTS------------------------------------";
		cout << kNumClients << " client threads." << endl;
		cout << kNumServers * kNumThreadsPerServer << " server threads (" << kNumServers << " ports running " << kNumThreadsPerServer << " threads each)." << endl;
		Logger(ORANGE) << "\n-------------------------------TIME--------------------------------------";
		
		cout << "total time: " << toucan_db::Client::kNumIterations << " requests @ " << double(ms2.count())/1000.0 << " seconds" << endl;
		
		double avgTimePerIteration = mcs.count() / double(toucan_db::Client::kNumIterations);
		cout << "avg time per itr: " << avgTimePerIteration << " microseconds (" << avgTimePerIteration / 1000 << " milliseconds)" << endl;
		
		double itrsPerSecond = chrono::microseconds::period::den / avgTimePerIteration;
		Logger(BLUE) << "Avg request rate: " << std::round(itrsPerSecond) << " requests/src";
		
		auto peakRequestRate = ((float)peakRequests / (chrono::duration_cast<chrono::milliseconds>(1 * kLogInterval).count())) * 1000.0;
		Logger(GREEN) << "Peak request rate: " << peakRequestRate << " requests/sec";
		
		Logger(ORANGE) << "Shutting down...";
	}
	
	this_thread::sleep_for(chrono::seconds(3));
	return 0;
}

//int main(int argc, char* arv[]) {
//	toucan_db::server::AsyncServer::Start();
//	this_thread::sleep_for(chrono::seconds(1));
//	return 0;
//}