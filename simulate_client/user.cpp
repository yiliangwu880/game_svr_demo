#include "user.h"

using namespace std;

const cfg &G_CFG = CfgMgr<cfg>::Obj().GetCfg();
namespace
{
	//简化解包操作。赋值并移动指针
	template<class T>
	void ParseCp(T &dst, const char *&src)
	{
		dst = *(T *)(src); // 类似 dst = *(uint32 *)(src)
		src = src + sizeof(dst);
	}
}
bool MyApp::OnStart()
{
	L_COND_F(UserMgr::Obj().Init());

	return true;
}

UserMgr::~UserMgr()
{
	for (auto &v: m_uin_2_user)
	{
		delete v.second;
	}
	m_uin_2_user.clear();
}

bool UserMgr::Init()
{

	L_INFO("init UserMgr, user num=%d", m_cfg.user_num);
	L_COND_F(m_cfg.user_num >= 1);

	for (uint32 i=0; i< m_cfg.user_num; ++i)
	{//create all user
		uint64 uin = m_cfg.user_uin_seg + i;
		SimulateUser *p = new SimulateUser(uin);
		m_uin_2_user[uin] = p;
	}

	m_sec_tm.StartTimer(1, std::bind(&UserMgr::OnSecLoop, this), true);
	return true;
}

void UserMgr::OnSecLoop()
{

}

SimulateUser::SimulateUser(uint64 uin)
	:m_con(*this)
{
	bool r = m_con.ConnectInit(G_CFG.acc_ex.ip.c_str(), G_CFG.acc_ex.port);
	L_COND(r);

}

void SimulateUser::Send(Cmd cmd, const google::protobuf::Message &msg)
{
	m_con.Send(cmd, msg);
}

void ClientConnecter::Send(Cmd cmd, const google::protobuf::Message &msg)
{
	string s;
	bool r = msg.SerializeToString(&s);
	L_COND(r, " msg.SerializeToString fail. ");


	string msg_pack;
	msg_pack.append((const char *)&cmd, sizeof(cmd));
	msg_pack.append(s);
	SendPack(msg_pack);
}

void Disapch(SimulateUser &user, Cmd cmd, const char *msg, uint16 msg_len)
{

}
void ClientConnecter::OnRecv(const lc::MsgPack &msg_pack)
{
	if (msg_pack.len < sizeof(Cmd))
	{
		L_WARN("illegal msg. msg.len=%d", msg_pack.len);
		return;
	}
	Cmd cmd;
	const char *cur = msg_pack.data;
	ParseCp(cmd, cur);
	const char *msg = cur;
	uint16 msg_len = msg_pack.len - sizeof(Cmd);
	switch (cmd)
	{
		Disapch(m_user, cmd, msg, msg_len);
	//case CMD_INIT_TABLE: Handle_CMD_INIT_TABLE(msg, msg_len); return;
	//case CMD_INSERT: Handle_CMD_INSERT(msg, msg_len); return;
	//case CMD_UPDATE: Handle_CMD_UPDATE(msg, msg_len); return;
	//case CMD_GET: Handle_CMD_GET(msg, msg_len); return;
	//case CMD_DEL: Handle_CMD_DEL(msg, msg_len); return;
	//case CMD_SQL: Handle_CMD_SQL(msg, msg_len); return;
	default:
		L_WARN("illegal msg. cmd=%d", cmd); return;
		break;
	}
}
