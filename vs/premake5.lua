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
	}
	
local COM_INCLUDE={ 
		"../external/",
		"../external/protobuf/include/",
		"../external/cpp_cfg/com/",
		"../proto/",
	}

Project "simulate_client"
	IncludeFile(COM_INCLUDE)

	SrcPath { 
		"../simulate_client/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../proto/**",
		"../external/cpp_cfg/com/**",
	}

Project "proto"
	files {
	"../proto/*.proto",
	}





