
#include "base_app.h"
#include "svr_util/include/stl_boost.h"
#include "libevent_cpp/include/include_all.h"
#include "CC_cfg.h"
#include "cs.pb.h"
#include "svr_util/include/static_trick/static_reg.h"


class SimulateUser;
using HandleMsg = void (*)(SimulateUser &user, const char *msg, uint16 msg_len);

REG_MAP_NAME_DECLARE(CmdHandleMap, Cmd, HandleMsg)



class MyApp : public BaseApp, public Singleton<MyApp>
{
private:
	virtual bool OnStart() override;

};


class ClientConnecter : public lc::ClientCon
{
	SimulateUser &m_user;
public:
	ClientConnecter(SimulateUser &user) :
		m_user(user)
	{

	}

	void Send(Cmd cmd, const google::protobuf::Message &msg);
private:
	virtual void OnRecv(const lc::MsgPack &msg) override;
	virtual void OnConnected() override
	{
		L_DEBUG("OnConnected");
		
	}
	virtual void OnDisconnected() override
	{
		L_DEBUG("OnDisconnected");

	}
};
//统计平均响应时间
struct ReplyTimeInfo
{
	uint32 total_wait_ms;//等待总时 ms
	uint32 total_cnt; //次数

};
class SimulateUser 
{
	friend ClientConnecter;

	enum State
	{
		S_WAIT_LOGIN,
		S_WAIT_ZONE_ECHO,//等zone 回显,
	};

	uint64 m_uin=0;
	State m_state= S_WAIT_LOGIN;
	//统计平均响应时间
	ReplyTimeInfo m_ze_rti; //zone echo reply time info
	ReplyTimeInfo m_fe_rti; //forward echo reply time info
	Cmd m_cur_cmd=CMD_NONE;

public:
	ClientConnecter m_con;

public:
	SimulateUser(uint64 uin);
	void Send(Cmd cmd, const google::protobuf::Message &msg);
private:

};

class UserMgr : public Singleton<UserMgr>
{
	std::map<uint64, SimulateUser*> m_uin_2_user;
	su::Timer m_sec_tm;
	cfg m_cfg;
public:
	~UserMgr();
	bool Init();

private:
	//每秒跑一次
	void OnSecLoop();
};

extern const cfg &G_CFG;
