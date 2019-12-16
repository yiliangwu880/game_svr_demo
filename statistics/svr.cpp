#include "svr.h"

using namespace lc;
using namespace std;
using namespace acc;
using namespace su;



void MyApp::AddLoginUser(uint64 uin, uint16 zone_id)
{
	COND(uin);
	auto iter = m_user_num.find(zone_id);
	if (iter == m_user_num.end())
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

	auto iter = m_user_num.find(zone_id);
	if (iter == m_user_num.end())
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
	for (auto &v: m_user_num)
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
	if (0 != session.uin)
	{
		MyApp::Obj().DisconUser(session.uin);
	}
}

void AccDriver::OnRevClientMsg(const Session &session, uint32 cmd, const char *msg, uint16 msg_len)
{

}
