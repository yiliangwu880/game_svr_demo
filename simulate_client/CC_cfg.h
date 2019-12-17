
//this file is build by CppCfg Tool
//don't modify by manual
#pragma once
#include <string>
#include <array>
#include "SimpleCfg.h"

struct cfg
{
	struct S_acc_ex
	{
		std::string ip;
		uint16_t port;
	};
	struct S_team
	{
		std::string echo_str;
		uint16_t max_echo_num_ps;
	};
	struct S_zone
	{
		std::string echo_str;
		uint16_t max_echo_num_ps;
	};


	////////////////////////define member list////////////////////////
	S_acc_ex acc_ex;
	bool is_daemon;
	uint16_t online_sec;
	S_team team;
	uint16_t user_num;
	uint16_t user_uin_seg;
	S_zone zone;

	////////////////////////method list////////////////////////
	//load or reload cfg file .
	bool LoadFile(const char *file_name=nullptr)
	{
		//default load original cfg file name
		if (nullptr == file_name)
		{
			file_name = "cfg.txt";
		}
		SimpleCfg js;
		if (!js.ParseFile(file_name))
		{
			return false;
		}
		return Assign(js);
	}

private:
	template<typename Array>
	inline size_t ArrayLen(const Array &array)
	{
		return sizeof(array) / sizeof(array[0]);
	}
	//initialize or reload cfg content.
	bool Assign(const nlohmann::json &js)
	{
		try
		{

			acc_ex.ip = js["acc_ex"]["ip"];
			acc_ex.port = js["acc_ex"]["port"];
			is_daemon = js["is_daemon"];
			online_sec = js["online_sec"];
			team.echo_str = js["team"]["echo_str"];
			team.max_echo_num_ps = js["team"]["max_echo_num_ps"];
			user_num = js["user_num"];
			user_uin_seg = js["user_uin_seg"];
			zone.echo_str = js["zone"]["echo_str"];
			zone.max_echo_num_ps = js["zone"]["max_echo_num_ps"];


			return true;
		}
		catch (...)
		{
			//if fail, pls check if your cfg fomart is legal.
			return false;
		}
	}
};
