
#include "base_app.h"
#include "svr_util/include/stl_boost.h"
#include "libevent_cpp/include/include_all.h"
#include "CC_cfg.h"
#include "cs.pb.h"
#include "svr_util/include/static_trick/static_reg.h"
#include "svr_util/include/string_tool.h"


class SimulateUser;
using HandleMsg = void (*)(SimulateUser &user, const char *msg, uint16 msg_len);

REG_MAP_NAME_DECLARE(CmdHandleMap, Cmd, HandleMsg)

extern const cfg &G_CFG;

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
	virtual void OnConnected() override;
	virtual void OnDisconnected() override
	{
		L_DEBUG("OnDisconnected");

	}
};
//ͳ��ƽ����Ӧʱ��
struct ReplyTimeInfo
{
	uint32 total_wait_us = 0;//�ȴ���ʱ us,΢��
	uint32 total_cnt=0; //����

};
class SimulateUser 
{
	friend ClientConnecter;

	enum State
	{
		S_WAIT_LOGIN,
		S_WAIT_ECHO,//��zone ����,
	};

	uint64 m_uin=0;
	State m_state= S_WAIT_LOGIN;
	Cmd m_cur_cmd=CMD_NONE;

	uint32 m_zone_echo_cnt_ps = 0; //ÿ��echo�������

	uint32 m_team_echo_cnt_ps = 0; //ÿ��echo�������

public:
	ClientConnecter m_con;

public:
	SimulateUser(uint64 uin);
	void Send(Cmd cmd, const google::protobuf::Message &msg);
	uint64 GetUin() { return m_uin; }
	void ReqZoneEchoFun();
	void ReqTeamEchoFun();
	uint64 CurTmUs(); //��ǰ΢�� ʱ��

	static void CMD_RspLogin(SimulateUser &user, const char *msg, uint16 msg_len);
	static void CMD_RspZoneEcho(SimulateUser &user, const char *msg, uint16 msg_len);
	static void CMD_RspTeamEcho(SimulateUser &user, const char *msg, uint16 msg_len);
	static void CMD_RspLoginZone(SimulateUser &user, const char *msg, uint16 msg_len);

	void  OnSec();
private:
};

class UserMgr : public Singleton<UserMgr>
{
	std::map<uint64, SimulateUser*> m_uin_2_user;
	su::Timer m_sec_tm;
	su::Timer m_10sec_tm;

public:
	//ͳ��ƽ����Ӧʱ��
	ReplyTimeInfo m_ze_rti; //zone echo reply time info
	ReplyTimeInfo m_te_rti; //team echo reply time info

public:
	~UserMgr();
	bool Init();

private:
	//ÿ����һ��
	void OnSecLoop();
	void On10SecLoop();
};

extern const cfg &G_CFG;
