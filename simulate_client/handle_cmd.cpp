#include "user.h"

using namespace std;
using namespace su;

#define REG_CMD_HANDLE(cmd)\
	MAP_REG_DEFINE(CmdHandleMap, cmd, H_##cmd);

#define REG_MEMBER_HANDLE(cmd)\
	MAP_REG_DEFINE(CmdHandleMap, cmd, SimulateUser::cmd);

void H_CMD_RspLogin(SimulateUser &user, const char *msg, uint16 msg_len)
{
	RspLogin rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));
	L_COND(rsp.is_ok());
	L_INFO("login ok. uin=%lld", user.GetUin());
}
REG_CMD_HANDLE(CMD_RspLogin); 


void SimulateUser::CMD_NtfLoginZone(SimulateUser &user, const char *msg, uint16 msg_len)
{
	NtfLoginZone rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));
	L_COND(rsp.uin() == user.GetUin());
	L_INFO("login zone ok. uin=%lld", user.GetUin());
	L_COND(user.m_state == S_WAIT_LOGIN);
	user.m_state = S_WAIT_ECHO;

}
REG_MEMBER_HANDLE(CMD_NtfLoginZone); 

void SimulateUser::CMD_RspZoneEcho(SimulateUser &user, const char *msg, uint16 msg_len)
{
	RspZoneEcho rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));
	L_COND(rsp.string().length() == G_CFG.zone.echo_str.length());

	L_COND(user.m_state == S_WAIT_ECHO);
	L_INFO("rev zone echo. uin=%lld", user.GetUin());

	
}
REG_MEMBER_HANDLE(CMD_RspZoneEcho);

void SimulateUser::CMD_RspTeamEcho(SimulateUser &user, const char *msg, uint16 msg_len)
{
	RspTeamEcho rsp;
	L_COND(rsp.ParseFromArray(msg, msg_len));
	L_COND(rsp.string().length() == G_CFG.team.echo_str.length());
	L_COND(user.m_state == S_WAIT_ECHO);
	L_INFO("rev team echo. uin=%lld", user.GetUin());
}
REG_MEMBER_HANDLE(CMD_RspTeamEcho);
