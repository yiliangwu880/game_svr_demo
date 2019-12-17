#include "svr.h"

using namespace lc;
using namespace std;
using namespace acc;
using namespace su;
using namespace ss;



void MyApp::AddLoginUser(uint64 uin, uint16 zone_id)
{
	COND(uin);
	auto iter = m_zoneid_2_user_num.find(zone_id);
	if (iter == m_zoneid_2_user_num.end())
	{
		L_ERROR("find zone_id fail. %x", zone_id);
		return;
	}
	iter->second++;
	
	if (!MapInsert(m_uin_2_zoneid, uin, zone_id))
	{
		L_ERROR("AddLoginUser fail. uin=%lld", uin);
	}
}

void MyApp::DisconUser(uint64 uin)
{
	COND(uin);
	uint32 zone_id = 0;
	{
		uint32 *p_zone_id = MapFind(m_uin_2_zoneid, uin);
		if (nullptr == p_zone_id)
		{
			L_ERROR("DisconUser uin fail. uin=%lld", uin);
			return;
		}
		zone_id = *p_zone_id;
	}
	if (!MapErase(m_uin_2_zoneid, uin))
	{
		L_ERROR("DisconUser uin fail. uin=%lld", uin);

	}

	auto iter = m_zoneid_2_user_num.find(zone_id);
	if (iter == m_zoneid_2_user_num.end())
	{
		L_ERROR("find zone_id fail. %x", zone_id);
		return;
	}
	if (iter->second == 0)
	{
		L_ERROR("user num error, zoneid= %x", zone_id);
		return;
	}
	iter->second--;
}

uint16 MyApp::GetLeastUserZone()
{
	uint32 min_num = std::numeric_limits<uint32>::max();
	uint16 svr_id = 0;
	for (auto &v: m_zoneid_2_user_num)
	{
		uint32 num = v.second;
		if (num < min_num)
		{
			min_num = num;
			svr_id = v.first;
		}
	}
	return svr_id;
}

void MyApp::AddZone(uint16 zone_id)
{
	L_COND(zone_id);
	bool r = MapInsert(m_zoneid_2_user_num, zone_id, 0);
	if (!r)
	{
		L_ERROR("repeated add zone fail.  zone_id=%x", zone_id);
	}
}

void MyApp::NtfStatistics(const NtfTeamStatistics &ntf)
{
	EchoStaticsInfo si;
	si.m_req_cnt = ntf.req_cnt();
	si.m_echo_total_bytes = ntf.echo_total_bytes();

	{
		uint64 bytes = (si.m_echo_total_bytes * 2) / ntf.use_sec();
		L_INFO("====team per second, req_cnt=%d, bytes=%lld,==========bytes=rev and send bytes, ===", si.m_req_cnt/10, bytes);
	}

}

void MyApp::NtfStatistics(const NtfZoneStatistics &ntf)
{
	auto *pV = MapFind(m_zoneid_2_si, ntf.zone_id());
	L_COND(nullptr == pV);
	L_COND(ntf.use_sec() == 10);

	EchoStaticsInfo &si = m_zoneid_2_si[ntf.zone_id()];
	si.m_req_cnt = ntf.req_cnt();
	si.m_echo_total_bytes = ntf.echo_total_bytes();
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

	uint32 zone_total_cnt = 0;
	uint64 zone_total_bytes = 0;
	for (auto &v: m_zoneid_2_si)
	{
		uint16 zoneid = v.first;
		const EchoStaticsInfo &si = v.second;
		uint64 bytes = (si.m_echo_total_bytes * 2) / 10;
		uint32 cnt_ps = si.m_req_cnt / 10;
		L_INFO("====zone avarage per second, zoneid, req, req_cnt, bytes=%x %d %lld,==========bytes=rev and send bytes, ===", zoneid, cnt_ps, bytes);
		zone_total_cnt += cnt_ps;
		zone_total_bytes += bytes;
	}
	L_INFO("====all zone avarage per second, req_cnt, bytes=%d %lld,==========bytes=rev and send bytes, ===", zone_total_cnt, zone_total_bytes);

	//clear statistics info
	m_zoneid_2_si.clear();

	for (auto &v : m_zoneid_2_user_num)
	{
		L_INFO("======zone online user. zone_id=%x, num=%lld", v.first, v.second);
	}
	L_INFO("======total online user num=%lld", m_uin_2_zoneid.size());
}

void AccDriver::SendToClient(const acc::SessionId &id, ::Cmd cmd, const google::protobuf::Message &msg)
{
	string s;
	bool r = msg.SerializeToString(&s);
	L_COND(r, " msg.SerializeToString fail. ");


	acc::ADFacadeMgr::SendToClient(id, cmd, s.c_str(), s.length());
	//L_DEBUG("s=%s", StringTool::BinaryToHex(s).c_str());

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
	if (0 != session.uin)
	{
		MyApp::Obj().DisconUser(session.uin);
	}
}

void AccDriver::OnRevClientMsg(const Session &session, uint32 cmd, const char *msg, uint16 msg_len)
{
	L_DEBUG("OnRevClientMsg cmd=%x", cmd);

	HandleMsg *pHandle = MapFind(HandleMsgMap::Obj(), cmd);
	if (nullptr == pHandle)
	{
		L_ERROR("find handle fail. cmd=%s", ::Cmd_Name(cmd).c_str());
		return;
	}
	(*pHandle)(session, cmd, msg, msg_len);
}



void HCMD_ReqLoginZone(const Session &session, uint32 cmd, const char *msg, uint16 msg_len)
{
	L_DEBUG("HCMD_ReqLoginZone");
	ReqLoginZone req;
	L_COND(req.ParseFromArray(msg, msg_len));

	uint16 zone_id = MyApp::Obj().GetLeastUserZone();
	L_COND(zone_id);
	//login zone, route zone msg
	{
		MyApp::Obj().AddLoginUser(req.uin(), zone_id);
		AccDriver::Obj().BroadcastUinToSession(session.id, req.uin());
		//zone1 id ӳ�� ����zone id
		AccDriver::Obj().SetMainCmd2Svr(session.id, ((ss::ST_ZONE<<8) + 1), zone_id);
	}
	//sent to client
	RspLoginZone rsp;
	rsp.set_uin(req.uin());
	AccDriver::Obj().SendToClient(session.id, CMD_RspLoginZone, rsp);

}


MAP_REG_DEFINE(HandleMsgMap, CMD_ReqLoginZone, HCMD_ReqLoginZone);