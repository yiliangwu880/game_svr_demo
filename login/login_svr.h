
#include "base_app.h"
#include "access_svr/acc_driver/include/acc_driver.h"
#include "svr_util/include/stl_boost.h"
#include "CC_cfg.h"
#include "cs.pb.h"

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
	cfg m_cfg;

public:
	std::set<uint64> m_user_id_set; //��֤�û�����
	//AccDriver acc_driver;
public:
	const cfg &GetCfg() const { return m_cfg; }
	const uint16 GetSvrId() const {return m_cfg.svr_id;}

private:
	virtual bool OnStart() override;

};


