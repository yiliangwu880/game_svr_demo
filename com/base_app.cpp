
#include "base_app.h"
#include <signal.h>

using namespace su;
using namespace lc;
using namespace std;


class MyLcLog : public lc::ILogPrinter, public Singleton<MyLcLog>
{
public:
	virtual void Printf(lc::LogLv lv, const char * file, int line, const char *fun, const char * pattern, va_list vp) override
	{
		su::LogMgr::Obj().Printf((su::LogLv)lv, file, line, fun, pattern, vp);
	}
};

void BaseApp::Run(int argc, char* argv[], const string &app_name)
{

	SuMgr::Obj().Init();

	//start or stop proccess
	SPMgr::Obj().Check(argc, argv, app_name.c_str(), BaseApp::OnExitProccess);

	EventMgr::Obj().Init(&MyLcLog::Obj());

	if (!OnStart())
	{
		L_INFO("start fail");
		return;
	}

	EventMgr::Obj().Dispatch();
	L_INFO("main end");
}


void BaseApp::SetDaemon()
{
	//��nochdirΪ0ʱ��daemon�����Ľ��ǵĸ�Ŀ¼Ϊroot(�� / ��)��
	//��nocloseΪ0�ǣ�daemon�����ǵ�STDIN, STDOUT, STDERR���ض��� / dev / null��
	if (0 != daemon(1, 0))
	{
		printf("daemon fail\n");
		return;
	}
}

void BaseApp::Exit()
{
	EventMgr::Obj().StopDispatch();
}

void BaseApp::OnExitProccess()
{
	L_INFO("OnExitProccess");
	EventMgr::Obj().StopDispatch();
}
