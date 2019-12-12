
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

void BaseApp::Run(int argc, char* argv[], const string &app_name, bool is_daemon)
{
	if (is_daemon)
	{
		//当nochdir为0时，daemon将更改进城的根目录为root(“ / ”)。
	    //当noclose为0是，daemon将进城的STDIN, STDOUT, STDERR都重定向到 / dev / null。
		if (0 != daemon(1, 0))
		{
			printf("daemon fail\n");
			return;
		}
	}
	SuMgr::Obj().Init();

	//start or stop proccess
	SingleProgress::Obj().Check(argc, argv, app_name.c_str());


	EventMgr::Obj().Init(&MyLcLog::Obj());
	m_tm_loop.StartTimer(1000, std::bind(BaseApp::OnOneSec), true);
	if (!OnStart())
	{
		L_INFO("start fail");
		return;
	}

	EventMgr::Obj().Dispatch();
	L_INFO("main end");
}



void BaseApp::Exit()
{
	EventMgr::Obj().StopDispatch();
}

void BaseApp::OnOneSec()
{
	SuMgr::Obj().OnTimer();
	if (SingleProgress::Obj().IsExit())
	{
		L_INFO("OnExitProccess");
		EventMgr::Obj().StopDispatch();
	}
}
