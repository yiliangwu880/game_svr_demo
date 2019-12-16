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

void MfDriver::On10Sec()
{
	if (!m_statistics_ready)
	{
		L_INFO("try con ID_STATISTIC");
		ConUser(ID_STATISTIC);
	}
}

void MfDriver::OnCon()
{
	L_INFO("reg mf ok.  svr_id=%x", G_CFG.svr_id);
	ConUser(ID_STATISTIC);
}

void MfDriver::OnDiscon()
{

}

void MfDriver::OnUserCon(uint32 dst_id)
{
	if (ID_STATISTIC == dst_id)
	{
		L_INFO("con ID_STATISTIC ok");
		m_statistics_ready = true;
		RegZone req;
		req.set_zone_id(G_CFG.svr_id);
		Send(ID_STATISTIC, CMD_RegZone, req);
	}
}

void MfDriver::OnUserDiscon(uint32 dst_id)
{
	if (ID_STATISTIC == dst_id)
	{
		L_ERROR("ID_STATISTIC discon");
		m_statistics_ready = false;
	}
}

void MfDriver::OnRecv(uint32 src_id, const char *custom_pack, uint16 custom_pack_len)
{
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

}
MAP_REG_DEFINE(MFHandleMsgMap, CMD_RegZone, HCMD_RegZone);