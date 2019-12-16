
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
	virtual void OnConnected() override;
	virtual void OnDisconnected() override
	{
		L_DEBUG("OnDisconnected");

	}
};
//ͳ��ƽ����Ӧʱ��
struct ReplyTimeInfo
{
	uint32 total_wait_ms;//�ȴ���ʱ ms
	uint32 total_cnt; //����

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
	//ͳ��ƽ����Ӧʱ��
	ReplyTimeInfo m_ze_rti; //zone echo reply time info
	ReplyTimeInfo m_fe_rti; //forward echo reply time info
	Cmd m_cur_cmd=CMD_NONE;

	su::Timer m_sec_tm; //�붨ʱ��
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

	static void CMD_NtfLoginZone(SimulateUser &user, const char *msg, uint16 msg_len);
	static void CMD_RspZoneEcho(SimulateUser &user, const char *msg, uint16 msg_len);
	static void CMD_RspTeamEcho(SimulateUser &user, const char *msg, uint16 msg_len);

private:
	void  OnSec();
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
	//ÿ����һ��
	void OnSecLoop();
};

extern const cfg &G_CFG;
