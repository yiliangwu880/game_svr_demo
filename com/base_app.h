
#include "libevent_cpp/include/include_all.h"
#include "svr_util/include/su_mgr.h"
#include "svr_util/include/single_progress.h"

#include "log_def.h"

//application base class
//启动命令:
//./app 启动
//./app stop 关闭
class BaseApp
{
public:
	//运行应用
	//@argc, argv . main函数的参数。参数表示： ./app 启动 ./app stop 关闭
	//@app_name 程序名称，用来区分同文件夹的程序
	//@is_daemon true表示后台运行
	void Run(int argc, char* argv[], const std::string &app_name);
	//后台执行
	void SetDaemon();
	//主动退出进程
	void Exit();

private:
	virtual bool OnStart() = 0;

private:
	//接收关闭信号USR1回调， 比如： kill -USR1 2581
	static void OnExitProccess();
};


