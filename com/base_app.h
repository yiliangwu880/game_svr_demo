#pragma once
#include "libevent_cpp/include/include_all.h"
#include "svr_util/include/su_mgr.h"
#include "svr_util/include/single_progress.h"
#include "global_cfg.h"
#include "log_def.h"

//application base class
//��������:
//./app ����
//./app stop �ر�
class BaseApp
{
	lc::Timer m_tm_loop;
public:
	//����Ӧ��
	//@argc, argv . main�����Ĳ�����������ʾ�� ./app ���� ./app stop �ر�
	//@app_name �������ƣ���������ͬ�ļ��еĳ���
	//@is_daemon true��ʾ��̨����
	void Run(int argc, char* argv[], const std::string &app_name, bool is_daemon=true);

	//�����˳�����
	void Exit();

private:
	//����Ӧ�ûص�
	//libevent_cpp������su������ʼ����󣬻ص�
	virtual bool OnStart() = 0;

private:
	//���չر��ź�USR1�ص��� ���磺 kill -USR1 2581
	static void OnOneSec();
};


