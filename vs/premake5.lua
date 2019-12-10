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
		"../bin/**.txt"
		"../bin/**.sh"
	}
	files {
	"../*.sh",
	"../*.txt",
	}
	

Project "dbproxy_svr"
	IncludeFile { 
		"../external/",
		"../external/protobuf/include/",
		"../proto/",
		"./cpp_cfg/com/",
	}

	SrcPath { 
		"../dbproxy_svr/**",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../proto/**",
		"./cpp_cfg/**",
	}
	files {
	"../*.txt",
	"../*.lua",
	}
	
    
    

Project "proto"
	files {
	"../proto/*.proto",
	}





