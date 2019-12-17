#include "user.h"

using namespace std;
using namespace su;

#define REG_CMD_HANDLE(cmd)\
	MAP_REG_DEFINE(CmdHandleMap, cmd, H_##cmd);

const cfg &G_CFG = CfgMgr<cfg>::Obj().GetCfg();

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

	L_INFO("init UserMgr, user num=%d", G_CFG.user_num);
	L_COND_F(G_CFG.user_num >= 1);

	for (uint32 i=0; i< G_CFG.user_num; ++i)
	{//create all user
		uint64 uin = G_CFG.user_uin_seg*10000 + i+1;
		SimulateUser *p = new SimulateUser(uin);
		m_uin_2_user[uin] = p;
	}

	m_sec_tm.StartTimer(1, std::bind(&UserMgr::OnSecLoop, this), true);
	m_10sec_tm.StartTimer(10, std::bind(&UserMgr::On10SecLoop, this), true);
	return true;
}

void UserMgr::OnSecLoop()
{
	for (auto &v : m_uin_2_user)
	{
		v.second->OnSec();
	}

}

void UserMgr::On10SecLoop()
{
	//最近10秒采样统计
	if (0 != m_ze_rti.total_cnt)
	{
		L_INFO("====zone 最近10秒采样，平均响应时间，总响应次数，总响应时间：%.2f ms %d %lld ====", double(m_ze_rti.total_wait_us / 1000) / m_ze_rti.total_cnt, m_ze_rti.total_cnt, m_ze_rti.total_wait_us);
	}
	if (0 != m_te_rti.total_cnt)
	{
		L_INFO("====team 最近10秒采样，平均响应时间，总响应次数，总响应时间：%.2f ms %d %lld ====", double(m_te_rti.total_wait_us / 1000) / m_te_rti.total_cnt, m_te_rti.total_cnt, m_te_rti.total_wait_us);
	}
	m_all_rti.total_cnt += m_ze_rti.total_cnt;
	m_all_rti.total_cnt += m_te_rti.total_cnt;
	m_all_rti.total_wait_us += m_ze_rti.total_wait_us;
	m_all_rti.total_wait_us += m_te_rti.total_wait_us;
	//所有采样统计
	L_INFO("====所有采样统计 平均响应时间，总响应次数：%.2f ms %d ====", double(m_all_rti.total_wait_us / 1000) / m_all_rti.total_cnt, m_all_rti.total_cnt);

	m_ze_rti.total_cnt = 0;
	m_ze_rti.total_wait_us = 0;
	m_te_rti.total_cnt = 0;
	m_te_rti.total_wait_us = 0;

}

SimulateUser::SimulateUser(uint64 uin)
	:m_uin(uin)
	,m_con(*this)
{
	bool r = m_con.ConnectInit(G_CFG.acc_ex.ip.c_str(), G_CFG.acc_ex.port);
	L_COND(r);

}

void SimulateUser::Send(Cmd cmd, const google::protobuf::Message &msg)
{
	m_con.Send(cmd, msg);
}



void SimulateUser::TryReqZoneEchoFun()
{
	L_COND(m_state == S_WAIT_ECHO);
	if (m_zone_echo_cnt_ps >= G_CFG.zone.max_echo_num_ps)
	{
		return;
	}
	m_zone_echo_cnt_ps++;

	ReqZoneEcho req;
	req.set_string(G_CFG.zone.echo_str);
	req.set_tm_us(CurTmUs());
	Send(CMD_ReqZoneEcho, req);
}

void SimulateUser::TryReqTeamEchoFun()
{
	L_COND(m_state == S_WAIT_ECHO);
	if (m_team_echo_cnt_ps >= G_CFG.team.max_echo_num_ps)
	{
		return;
	}
	m_team_echo_cnt_ps++;

	ReqTeamEcho req;
	req.set_string(G_CFG.team.echo_str);
	req.set_tm_us(CurTmUs());
	Send(CMD_ReqTeamEcho, req);

	//L_DEBUG("TryReqTeamEchoFun CurTmUs()=%lld", CurTmUs());
}


uint64 SimulateUser::CurTmUs()
{
	struct  timeval start;
	gettimeofday(&start, NULL);

	return 1000000 * (start.tv_sec) + start.tv_usec;
}

void SimulateUser::OnSec()
{
	m_zone_echo_cnt_ps = 0;
	m_team_echo_cnt_ps = 0;
	if (m_state == S_WAIT_ECHO)
	{
		TryReqZoneEchoFun();
		TryReqTeamEchoFun();
	}
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

	HandleMsg *pHandle = MapFind(CmdHandleMap::Obj(), cmd);
	if (nullptr == pHandle)
	{
		L_ERROR("find handle fail. cmd=%s", Cmd_Name(cmd).c_str());
		return;
	}
	m_user.m_cur_cmd = cmd;
	(*pHandle)(m_user, msg, msg_len);
}

void ClientConnecter::OnConnected()
{
	//L_DEBUG("OnConnected uin=%lld", m_user.m_uin);
	ReqLogin req;
	req.set_user_uin(m_user.m_uin);
	req.set_is_verify_ok(true);
	Send(CMD_ReqLogin, req);
}
