REM 复制远程linux proto导出文件到win目录

xcopy Z:\test\game_svr_demo\proto_cpp\*.h ..\proto_cpp\ /sy
xcopy Z:\test\game_svr_demo\proto_cpp\*.cc ..\proto_cpp\ /sy
xcopy Z:\test\game_svr_demo\login\CC*.h ..\login\ /sy
xcopy Z:\test\game_svr_demo\simulate_client\CC*.h ..\simulate_client\ /sy
xcopy Z:\test\game_svr_demo\zone\CC*.h ..\zone\ /sy
xcopy Z:\test\game_svr_demo\team\CC*.h ..\team\ /sy
xcopy Z:\test\game_svr_demo\statistics\CC*.h ..\statistics\ /sy


REM xcopy Z:\test\game_svr_demo\dbproxy_svr\cpp_cfg\*.h E:\my\game_svr_demo\dbproxy_svr\cpp_cfg\ /sy

pause