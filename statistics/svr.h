
#include "base_app.h"
#include "access_svr/acc_driver/include/acc_driver.h"
#include "svr_util/include/stl_boost.h"
#include "svr_util/include/string_tool.h"
#include "CC_cfg.h"
#include "cs.pb.h"
#include "ss.pb.h"
#include "mf.h"

using namespace std;
using HandleMsg = void(*)(const acc::Session &session, uint32 cmd, const char *msg, uint16 msg_len);
REG_MAP_NAME_DECLARE(HandleMsgMap, uint32, HandleMsg)

class AccDriver : public acc::ADFacadeMgr, public Singleton<AccDriver>
{
public:

	void SendToClient(const acc::SessionId &id, ::Cmd cmd, const google::protobuf::Message &msg);

private:
	//回调注册结果, 失败就是配置错误了，无法修复。重启进程吧。
	//@svr_id = 0表示失败
	virtual void OnRegResult(uint16 svr_id);

	//client断线通知
	virtual void OnClientDisCon(const acc::Session &session);

	//接收client消息包到svr
	virtual void OnRevClientMsg(const acc::Session &session, uint32 cmd, const char *msg, uint16 msg_len);
};


struct EchoStaticsInfo 
{
	uint32 m_req_cnt = 0; //处理请求次数
	uint64 m_echo_total_bytes = 0;//响应回显总字节数
};

class MyApp : public BaseApp, public Singleton<MyApp>
{
	std::map<uint16, uint32> m_zoneid_2_user_num; //zone svrid 2 user num
	std::map<uint64, uint32> m_uin_2_zoneid; //uin 2 zone svrid
	su::Timer m_tm_10sec;
	std::map <uint16, EchoStaticsInfo> m_zoneid_2_si;

public:

	bool IsLoginUser(uint64 uin);
	void AddLoginUser(uint64 uin, uint16 zone_id);
	void DisconUser(uint64 uin);
	uint16 GetLeastUserZone(); 
	void AddZone(uint16 zone_id);

	void NtfStatistics(const ss::NtfTeamStatistics &ntf);
	void NtfStatistics(const ss::NtfZoneStatistics &ntf);
private:
	virtual bool OnStart() override;

	void On10Sec();

};


