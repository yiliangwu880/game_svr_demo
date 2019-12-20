
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
	std::vector<S_acc_inner> acc_inner;
	bool is_daemon;
	std::vector<S_mf_svr> mf_svr;
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

			for (size_t i = 0; i < js["acc_inner"].size(); ++i)
			{
				acc_inner.push_back({});
				acc_inner[i].ip = js["acc_inner"][i]["ip"];
				acc_inner[i].port = js["acc_inner"][i]["port"];
			}
			is_daemon = js["is_daemon"];
			for (size_t i = 0; i < js["mf_svr"].size(); ++i)
			{
				mf_svr.push_back({});
				mf_svr[i].ip = js["mf_svr"][i]["ip"];
				mf_svr[i].port = js["mf_svr"][i]["port"];
			}
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
