--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--
require "lib"

local action = _ACTION or ""
local outdir = action


WorkSpaceInit  "game_svr_demo"

Project "game_svr_demo"

	SrcPath { 
		"../bin/**.txt",
		"../bin/**.sh",
	}
	files {
	"../*.sh",
	"../*.txt",
	"../vs/premake5.lua",
	"../bin/**.txt",
	"../bin/**.sh",
	"../tool/**.sh",
	}
	
local COM_INCLUDE={ 
		"../external/",
		"../external/protobuf/include/",
		"../external/cpp_cfg/com/",
		"../proto_cpp/",
		"../com/",
	}


Project "proto"
	files {
	"../proto/*.proto",
	}
--Project "proto_cpp"
--	files {
--	"../proto_cpp/**.h",
--	"../proto_cpp/**.cc",
--	}


function BuildPrj(prj_name)
	Project(prj_name)
		IncludeFile(COM_INCLUDE)

		SrcPath { 
			"../"..prj_name.."/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
			--"../proto_cpp/**",
			"../external/cpp_cfg/com/**",
			"../com/**",
		}
end

BuildPrj("simulate_client")
BuildPrj("login")
BuildPrj("statistics")
BuildPrj("team")
BuildPrj("zone")

