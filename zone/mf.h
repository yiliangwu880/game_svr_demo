
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
	bool m_statistics_ready = false; //true 表示。 依赖的 statistics准备好
public:
	bool Init();

	void Send(uint32 svr_id, ss::Cmd cmd, const google::protobuf::Message &msg);
	void On10Sec();
private:
	//反馈连接mf svr list 情况。连接任意第一台都算成功。
	//连接已成功的情况，再连接第N台，不会回调。
	virtual void OnCon() override;
	//全部连接都失败就反馈。
	virtual void OnDiscon() override;
	//请求 ConUser 目标后，成功回调
	virtual void OnUserCon(uint32 dst_id) override;
	//以下情况导致失败都会回调
	//链接对方，对方主动断线， 发送消息给对方,
	virtual void OnUserDiscon(uint32 dst_id) override;
	//@para src_id 发送方服务器id
	virtual void OnRecv(uint32 src_id, const char *custom_pack, uint16 custom_pack_len) override;
};


