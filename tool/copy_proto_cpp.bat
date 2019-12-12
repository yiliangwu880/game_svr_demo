REM 复制远程linux proto导出文件到win目录

xcopy Z:\test\game_svr_demo\proto_cpp\*.h ..\proto_cpp\ /sy
xcopy Z:\test\game_svr_demo\proto_cpp\*.cc ..\proto_cpp\ /sy
xcopy Z:\test\game_svr_demo\bin\login1\*.h ..\login\ /sy
xcopy Z:\test\game_svr_demo\bin\simulate_client1\*.h ..\simulate_client\ /sy


REM xcopy Z:\test\game_svr_demo\dbproxy_svr\cpp_cfg\*.h E:\my\game_svr_demo\dbproxy_svr\cpp_cfg\ /sy

pause