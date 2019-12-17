#include "user.h"

using namespace std;
using namespace su;

#define REG_CMD_HANDLE(cmd)\
	MAP_REG_DEFINE(CmdHandleMap, cmd, H_##cmd);

#define REG_MEMBER_HANDLE(cmd)\
	MAP_REG_DEFINE(CmdHandleMap, cmd, SimulateUser::cmd);

void SimulateUser::CMD_RspLogin(SimulateUser &user, const char *msg, uint16 msg_len)
{
	RspLogin rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));
	L_COND(rsp.is_ok());
	L_INFO("login ok. ReqLoginZone, uin=%lld", user.GetUin());

	ReqLoginZone req;
	req.set_uin(user.GetUin());
	user.m_con.Send(CMD_ReqLoginZone, req);
}



void SimulateUser::CMD_RspZoneEcho(SimulateUser &user, const char *msg, uint16 msg_len)
{
	RspZoneEcho rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));
	L_COND(rsp.string().length() == G_CFG.zone.echo_str.length());

	L_COND(user.m_state == S_WAIT_ECHO);
	//L_INFO("rev zone echo. uin=%lld", user.GetUin());

	L_COND(user.CurTmUs() > rsp.tm_us());
	uint64 wait_us = user.CurTmUs() - rsp.tm_us();
	if (wait_us/(1000*1000)> 10)//>10sec
	{
		L_ERROR("rsp wait too long. > 10 second");
		return;
	}
	//L_DEBUG("CMD_RspZoneEcho wait_us=%lld", wait_us);
	UserMgr::Obj().m_ze_rti.total_cnt++;
	UserMgr::Obj().m_ze_rti.total_wait_us += wait_us;
	user.TryReqZoneEchoFun();
}

void SimulateUser::CMD_RspTeamEcho(SimulateUser &user, const char *msg, uint16 msg_len)
{
	RspTeamEcho rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));
	L_COND(rsp.string().length() == G_CFG.team.echo_str.length());
	L_COND(user.m_state == S_WAIT_ECHO);
	//L_INFO("rev team echo. uin=%lld", user.GetUin());

	L_COND(user.CurTmUs() > rsp.tm_us());
	uint64 wait_us = user.CurTmUs() - rsp.tm_us();
	//L_DEBUG("CMD_RspTeamEcho CurTmUs()=%lld, wait_us=%lld", user.CurTmUs(), wait_us);
	if (wait_us /(1000*1000) > 30)//>10sec
	{
		L_ERROR("rsp wait too long. > 30 second");
		return;
	}
	//L_DEBUG("CMD_RspTeamEcho wait_us=%lld", wait_us);
	UserMgr::Obj().m_te_rti.total_cnt++;
	UserMgr::Obj().m_te_rti.total_wait_us += wait_us;
	user.TryReqTeamEchoFun();
}


void SimulateUser::CMD_RspLoginZone(SimulateUser &user, const char *msg, uint16 msg_len)
{
//	L_DEBUG("msg=%s, len=%d", StringTool::BinaryToHex(string(msg, msg_len)).c_str(), msg_len);
	RspLoginZone rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));

	L_INFO("login zone ok. uin=%lld", user.GetUin());
	L_COND(user.m_state == S_WAIT_LOGIN);
	user.m_state = S_WAIT_ECHO;

}
REG_MEMBER_HANDLE(CMD_RspLogin);
REG_MEMBER_HANDLE(CMD_RspZoneEcho);
REG_MEMBER_HANDLE(CMD_RspTeamEcho);
REG_MEMBER_HANDLE(CMD_RspLoginZone);
