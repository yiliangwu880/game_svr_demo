
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

private:
	//�ص�ע����, ʧ�ܾ������ô����ˣ��޷��޸����������̰ɡ�
	//@svr_id = 0��ʾʧ��
	virtual void OnRegResult(uint16 svr_id);
	//����client��Ϣ��.������֤�İ�
	virtual void OnRevVerifyReq(const acc::SessionId &id, uint32 cmd, const char *msg, uint16 msg_len) override;

	//client����֪ͨ
	virtual void OnClientDisCon(const acc::Session &session);
};



class MyApp : public BaseApp, public Singleton<MyApp>
{

public:
	std::set<uint64> m_user_id_set; //��֤�û�����

public:

private:
	virtual bool OnStart() override;

};


