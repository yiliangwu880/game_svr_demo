
#include "base_app.h"
#include "access_svr/acc_driver/include/acc_driver.h"
#include "svr_util/include/stl_boost.h"
#include "CC_cfg.h"
#include "cs.pb.h"
#include "ss.pb.h"
#include "mf.h"

using namespace std;


class AccDriver : public acc::ADFacadeMgr, public Singleton<AccDriver>
{
public:

	void SendToClient(const acc::SessionId &id, ::Cmd cmd, const google::protobuf::Message &msg);

private:
	//�ص�ע����, ʧ�ܾ������ô����ˣ��޷��޸����������̰ɡ�
	//@svr_id = 0��ʾʧ��
	virtual void OnRegResult(uint16 svr_id);

	//client����֪ͨ
	virtual void OnClientDisCon(const acc::Session &session);

	//����client��Ϣ����svr
	virtual void OnRevClientMsg(const acc::Session &session, uint32 cmd, const char *msg, uint16 msg_len);
};



class MyApp : public BaseApp, public Singleton<MyApp>
{
	std::map<uint16, uint32> m_user_num; //zone svrid 2 user num
	std::map<uint64, uint32> m_uin_2_zoneid; //uin 2 zone svrid
public:

	void AddLoginUser(uint64 uin, uint16 zone_id);
	void DisconUser(uint64 uin);
	uint16 GetLeastUserZone(); 

private:
	virtual bool OnStart() override;

};


