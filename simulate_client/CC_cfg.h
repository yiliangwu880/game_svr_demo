
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


	////////////////////////define member list////////////////////////
	S_acc_ex acc_ex;
	uint16_t fowrad_echo_sec;
	bool is_daemon;
	uint16_t online_sec;
	uint16_t user_num;
	uint16_t user_uin_seg;

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
			fowrad_echo_sec = js["fowrad_echo_sec"];
			is_daemon = js["is_daemon"];
			online_sec = js["online_sec"];
			user_num = js["user_num"];
			user_uin_seg = js["user_uin_seg"];


			return true;
		}
		catch (...)
		{
			//if fail, pls check if your cfg fomart is legal.
			return false;
		}
	}
};
