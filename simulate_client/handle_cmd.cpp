#include "user.h"

using namespace std;
using namespace su;

#define REG_CMD_HANDLE(cmd)\
	MAP_REG_DEFINE(CmdHandleMap, cmd, H_##cmd);

void H_CMD_ReqLogin(SimulateUser &user, const char *msg, uint16 msg_len)
{

}
REG_CMD_HANDLE(CMD_ReqLogin);