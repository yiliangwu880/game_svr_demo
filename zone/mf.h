
#pragma once
#include "base_app.h"
#include "msg_forward/mf_driver/include/mf_driver.h"
#include "svr_util/include/stl_boost.h"
#include "svr_util/include/static_trick/static_reg.h"
#include "access_svr/acc_driver/include/acc_driver.h"
#include "CC_cfg.h"
#include "cs.pb.h"
#include "ss.pb.h"

using MFHandleMsg = void(*)(uint32 svr_id, const char *msg, uint16 msg_len);
REG_MAP_NAME_DECLARE(MFHandleMsgMap, ss::Cmd, MFHandleMsg)

extern const cfg &G_CFG;

class MfDriver : public mf::MfClientMgr, public Singleton<MfDriver>
{
	bool m_statistics_ready = false; //true ��ʾ�� ������ statistics׼����
public:
	bool Init();

	void Send(uint32 svr_id, ss::Cmd cmd, const google::protobuf::Message &msg);
	void On10Sec();
private:
	//��������mf svr list ��������������һ̨����ɹ���
	//�����ѳɹ�������������ӵ�N̨������ص���
	virtual void OnCon() override;
	//ȫ�����Ӷ�ʧ�ܾͷ�����
	virtual void OnDiscon() override;
	//���� ConUser Ŀ��󣬳ɹ��ص�
	virtual void OnUserCon(uint32 dst_id) override;
	//�����������ʧ�ܶ���ص�
	//���ӶԷ����Է��������ߣ� ������Ϣ���Է�,
	virtual void OnUserDiscon(uint32 dst_id) override;
	//@para src_id ���ͷ�������id
	virtual void OnRecv(uint32 src_id, const char *custom_pack, uint16 custom_pack_len) override;
};


