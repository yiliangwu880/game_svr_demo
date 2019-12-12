

#include "user.h"

using namespace std;


int main(int argc, char* argv[])
{
	if (!CfgMgr<cfg>::Obj().Init("cfg.txt"))
	{
		printf("init cfg fail");
		return false;
	}
	MyApp::Obj().Run(argc, argv, "simulate_client", G_CFG.is_daemon);
	return 0;
}

