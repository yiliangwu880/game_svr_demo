/*
use example:

h file:
extern const cfg &G_CFG;

cpp file:
const cfg &G_CFG = CfgMgr<cfg>::Obj().GetCfg();

int main(int argc, char* argv[])
{
	if (!CfgMgr<cfg>::Obj().Init("cfg.txt"))
	{
		printf("init cfg fail");
		return false;
	}
	G_CFG.is_daemon;
	return 0;
}
*/

#pragma once
#include "libevent_cpp/include/include_all.h"
#include "svr_util/include/su_mgr.h"
#include "svr_util/include/single_progress.h"
#include "log_def.h"

template<class Cfg>
class CfgMgr 
{
public:
	static CfgMgr &Obj()
	{
		static CfgMgr obj;
		return obj;
	}
	bool Init(const char *CFG_FILE_NAME)
	{
		L_COND_F(m_cfg.LoadFile(CFG_FILE_NAME));
		return true;
	}
	const Cfg &GetCfg() { return m_cfg; }

private:
	Cfg m_cfg;
};

