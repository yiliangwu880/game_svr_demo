
#include "base_app.h"
#include "access_svr/acc_driver/include/acc_driver.h"
#include "svr_util/include/stl_boost.h"
#include "CC_cfg.h"
#include "cs.pb.h"
#include "ss.pb.h"
#include "mf.h"

using namespace std;


class AccDriver : public acc::ADFacadeMgr, public Singleton<AccDriver>
{

private:
	//回调注册结果, 失败就是配置错误了，无法修复。重启进程吧。
	//@svr_id = 0表示失败
	virtual void OnRegResult(uint16 svr_id);
	//接收client消息包.请求认证的包
	virtual void OnRevVerifyReq(const acc::SessionId &id, uint32 cmd, const char *msg, uint16 msg_len) override;

	//client断线通知
	virtual void OnClientDisCon(const acc::Session &session);
};



class MyApp : public BaseApp, public Singleton<MyApp>
{

public:
	std::set<uint64> m_user_id_set; //验证用户集合

public:

private:
	virtual bool OnStart() override;

};


