
#include "base_app.h"

using namespace std;

class MyApp : public BaseApp, public Singleton<MyApp>
{
private:
	virtual bool OnStart() override;

};

bool MyApp::OnStart()
{


	return true;
}

int main(int argc, char* argv[])
{
	MyApp::Obj().Run(argc, argv, "simulate_client");
	return 0;
}

