#include "login_svr.h"

using namespace lc;
using namespace std;
using namespace acc;
using namespace ss;


bool MyApp::OnStart()
{

	vector<acc::Addr> vec_addr;
	for (const cfg::S_acc_inner &v: G_CFG.acc_inner)
	{
		vec_addr.push_back({ v.ip, v.port });
	}

	L_COND_F(AccDriver::Obj().Init(vec_addr, G_CFG.svr_id, true));

	L_COND_F(MfDriver::Obj().Init());

	return true;
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

void AccDriver::OnRevVerifyReq(const SessionId &id, uint32 cmd, const char *msg, uint16 msg_len)
{
	L_COND(MfDriver::Obj().IsCon(), "mf is not prepare");
	L_INFO("handle verify");
	L_COND(cmd == CMD_ReqLogin);
	ReqLogin req; 
	L_COND(req.ParseFromArray(msg, msg_len));

	RspLogin rsp;
	rsp.set_is_ok(req.is_verify_ok());
	string rsp_str;
	rsp.SerializeToString(&rsp_str);
	ReqVerifyRet(id, req.is_verify_ok(), CMD_RspLogin, rsp_str.c_str(), rsp_str.length());
	if (!req.is_verify_ok())
	{
		return;
	}

	bool r = MyApp::Obj().m_user_id_set.insert(req.user_uin()).second;
	if (!r)
	{
		L_ERROR("repeated login user. uin=%lld", req.user_uin());
	}

}

void AccDriver::OnClientDisCon(const Session &session)
{
	if (0 != session.uin)
	{
		MyApp::Obj().m_user_id_set.erase(session.uin);
	}
	else
	{
		L_ERROR("discon a un login client.  uin=0");
	}
}
