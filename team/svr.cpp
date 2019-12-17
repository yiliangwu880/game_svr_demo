#include "svr.h"

using namespace lc;
using namespace std;
using namespace acc;
using namespace su;
using namespace ss;




void MyApp::StatisticEcho(uint64 msg_len)
{
	m_req_cnt++;
	m_echo_total_bytes += msg_len;
}

bool MyApp::OnStart()
{

	vector<acc::Addr> vec_addr;
	for (const cfg::S_acc_inner &v: G_CFG.acc_inner)
	{
		vec_addr.push_back({ v.ip, v.port });
	}

	L_INFO("svr id=%x", G_CFG.svr_id);
	L_COND_F(AccDriver::Obj().Init(vec_addr, G_CFG.svr_id));

	L_COND_F(MfDriver::Obj().Init());

	m_tm_10sec.StartTimer(10, std::bind(&MyApp::On10Sec, this), true);

	return true;
}


void MyApp::On10Sec()
{
	if (!MfDriver::Obj().IsCon())
	{
		return;
	}
	NtfTeamStatistics ntf;
	ntf.set_req_cnt(m_req_cnt);
	ntf.set_echo_total_bytes(m_echo_total_bytes);
	ntf.set_use_sec(10);
	MfDriver::Obj().Send(ID_STATISTIC, CMD_NtfTeamStatistics, ntf);
	m_req_cnt = 0;
	m_echo_total_bytes = 0;
}

void AccDriver::SendToClient(const acc::SessionId &id, ::Cmd cmd, const google::protobuf::Message &msg)
{
	string s;
	bool r = msg.SerializeToString(&s);
	L_COND(r, " msg.SerializeToString fail. ");

	acc::ADFacadeMgr::SendToClient(id, cmd, s.c_str(), s.length());
}

void AccDriver::OnRegResult(uint16 svr_id)
{
	if (0 == svr_id)
	{
		LB_FATAL("reg acc fail, exit progress");
		MyApp::Obj().Exit();
	}
	L_COND(svr_id == G_CFG.svr_id);
	L_INFO("reg acc ok");
}


void AccDriver::OnClientDisCon(const Session &session)
{

}

void AccDriver::OnRevClientMsg(const Session &session, uint32 cmd, const char *msg, uint16 msg_len)
{

	HandleMsg *pHandle = MapFind(HandleMsgMap::Obj(), cmd);
	if (nullptr == pHandle)
	{
		L_ERROR("find handle fail. cmd=%x", cmd);
		return;
	}
	(*pHandle)(session, cmd, msg, msg_len);
}


void HCMD_ReqTeamEcho(const Session &session, uint32 cmd, const char *msg, uint16 msg_len)
{
	ReqTeamEcho req;
	L_COND(req.ParseFromArray(msg, msg_len));

	RspTeamEcho rsp;
	rsp.set_string(req.string());
	rsp.set_tm_us(req.tm_us());

	AccDriver::Obj().SendToClient(session.id, CMD_RspTeamEcho, rsp);
	MyApp::Obj().StatisticEcho(rsp.ByteSize());
}
MAP_REG_DEFINE(HandleMsgMap, CMD_ReqTeamEcho, HCMD_ReqTeamEcho);