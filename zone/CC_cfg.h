
//this file is build by CppCfg Tool
//don't modify by manual
#pragma once
#include <string>
#include <array>
#include "SimpleCfg.h"

struct cfg
{
	struct S_acc_inner
	{
		std::string ip;
		uint16_t port;
	};
	struct S_mf_svr
	{
		std::string ip;
		uint16_t port;
	};


	////////////////////////define member list////////////////////////
	std::array<S_acc_inner,4> acc_inner;
	bool is_daemon;
	std::array<S_mf_svr,2> mf_svr;
	int16_t svr_id;

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

			acc_inner[0].ip = js["acc_inner"][0]["ip"];
			acc_inner[0].port = js["acc_inner"][0]["port"];
			acc_inner[1].ip = js["acc_inner"][1]["ip"];
			acc_inner[1].port = js["acc_inner"][1]["port"];
			acc_inner[2].ip = js["acc_inner"][2]["ip"];
			acc_inner[2].port = js["acc_inner"][2]["port"];
			acc_inner[3].ip = js["acc_inner"][3]["ip"];
			acc_inner[3].port = js["acc_inner"][3]["port"];
			is_daemon = js["is_daemon"];
			mf_svr[0].ip = js["mf_svr"][0]["ip"];
			mf_svr[0].port = js["mf_svr"][0]["port"];
			mf_svr[1].ip = js["mf_svr"][1]["ip"];
			mf_svr[1].port = js["mf_svr"][1]["port"];
			svr_id = js["svr_id"];


			return true;
		}
		catch (...)
		{
			//if fail, pls check if your cfg fomart is legal.
			return false;
		}
	}
};
