#include "svr.h"

using namespace lc;
using namespace std;
using namespace acc;
using namespace su;




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


void AccDriver::SendToClient(const acc::SessionId &id, ::Cmd cmd, const google::protobuf::Message &msg)
{
	string s;
	bool r = msg.SerializeToString(&s);
	L_COND(r, " msg.SerializeToString fail. ");

	string msg_pack;
	msg_pack.append((const char *)&cmd, sizeof(cmd));
	msg_pack.append(s);
	acc::ADFacadeMgr::SendToClient(id, cmd, msg_pack.c_str(), msg_pack.length());
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

}
