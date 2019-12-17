
#include "base_app.h"
#include "access_svr/acc_driver/include/acc_driver.h"
#include "svr_util/include/stl_boost.h"
#include "CC_cfg.h"
#include "cs.pb.h"
#include "ss.pb.h"
#include "mf.h"

using namespace std;

using HandleMsg = void(*)(const acc::Session &session, uint32 cmd, const char *msg, uint16 msg_len);
REG_MAP_NAME_DECLARE(HandleMsgMap, uint32, HandleMsg)

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
	su::Timer m_tm_10sec;
	uint32 m_req_cnt; //�����������
	uint64 m_echo_total_bytes;//��Ӧ�������ֽ���

public:

	void StatisticEcho(uint64 msg_len);

private:
	virtual bool OnStart() override;

	void On10Sec();

};


