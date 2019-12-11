REM 复制远程linux proto导出文件到win目录

xcopy Z:\test\game_svr_demo\proto_cpp\*.h ..\proto_cpp\ /sy
xcopy Z:\test\game_svr_demo\proto_cpp\*.cc E:\my\game_svr_demo\proto_cpp\ /sy
REM xcopy Z:\test\game_svr_demo\dbproxy_svr\cpp_cfg\*.h E:\my\game_svr_demo\dbproxy_svr\cpp_cfg\ /sy

pause