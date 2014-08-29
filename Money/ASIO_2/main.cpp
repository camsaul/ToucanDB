

#include "Storage.h"
#include "AsyncServer.h"

int main(int argc, char* arv[]) {
	toucan_db::server::AsyncServer::Start();
		
	return 0;
}