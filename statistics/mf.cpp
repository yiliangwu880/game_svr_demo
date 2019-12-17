#include "mf.h"
#include "svr.h"

using namespace lc;
using namespace std;
using namespace mf;
using namespace ss;
using namespace acc;
using namespace su;

const cfg &G_CFG = CfgMgr<cfg>::Obj().GetCfg();

	

bool MfDriver::Init()
{
	vector<MfAddr> vec;
	for (auto &v : G_CFG.mf_svr)
	{
		vec.push_back({ v.ip, v.port });
	}

	return MfClientMgr::Init(vec, (uint32)G_CFG.svr_id);
}


void MfDriver::Send(uint32 svr_id, ss::Cmd cmd, const google::protobuf::Message &msg)
{
	string s;
	bool r = msg.SerializeToString(&s);
	L_COND(r, " msg.SerializeToString fail. ");


	string msg_pack;
	msg_pack.append((const char *)&cmd, sizeof(cmd));
	msg_pack.append(s);
	MfClientMgr::Send(svr_id, msg_pack.c_str(), msg_pack.length());
}

void MfDriver::OnCon()
{
	L_INFO("reg mf ok.  svr_id=%x", G_CFG.svr_id);
}

void MfDriver::OnDiscon()
{

}

void MfDriver::OnUserCon(uint32 dst_id)
{

}

void MfDriver::OnUserDiscon(uint32 dst_id)
{

}

void MfDriver::OnRecv(uint32 src_id, const char *custom_pack, uint16 custom_pack_len)
{
//	L_DEBUG("MfDriver::OnRecv, src_id=%x, custom_pack_len=%d", src_id, custom_pack_len);
	if (custom_pack_len < sizeof(ss::Cmd))
	{
		L_WARN("illegal msg. msg.len=%d", custom_pack_len);
		return;
	}
	ss::Cmd cmd;
	const char *cur = custom_pack;
	ParseCp(cmd, cur);
	const char *msg = cur;
	uint16 msg_len = custom_pack_len - sizeof(ss::Cmd);

	MFHandleMsg *pHandle = MapFind(MFHandleMsgMap::Obj(), cmd);
	if (nullptr == pHandle)
	{
		L_ERROR("find handle fail. cmd=%s", ss::Cmd_Name(cmd).c_str());
		return;
	}
	(*pHandle)(src_id, msg, msg_len);
}



void HCMD_RegZone(uint32 svr_id, const char *msg, uint16 msg_len)
{
	RegZone ntf;
	L_COND(ntf.ParseFromArray(msg, msg_len));
	MyApp::Obj().AddZone(ntf.zone_id());
}

void HCMD_NtfTeamStatistics(uint32 svr_id, const char *msg, uint16 msg_len)
{
	NtfTeamStatistics ntf;
	L_COND(ntf.ParseFromArray(msg, msg_len));
	MyApp::Obj().NtfStatistics(ntf);

}

void HCMD_NtfZoneStatistics(uint32 svr_id, const char *msg, uint16 msg_len)
{
	NtfZoneStatistics ntf;
	L_COND(ntf.ParseFromArray(msg, msg_len));
	MyApp::Obj().NtfStatistics(ntf);
}


MAP_REG_DEFINE(MFHandleMsgMap, CMD_RegZone, HCMD_RegZone);
MAP_REG_DEFINE(MFHandleMsgMap, CMD_NtfTeamStatistics, HCMD_NtfTeamStatistics);
MAP_REG_DEFINE(MFHandleMsgMap, CMD_NtfZoneStatistics, HCMD_NtfZoneStatistics);
