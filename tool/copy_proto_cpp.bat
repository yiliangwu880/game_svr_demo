REM 复制远程linux proto导出文件到win目录

xcopy Z:\test\game_svr_demo\proto\*.h ..\proto\ /sy
xcopy Z:\test\game_svr_demo\proto\*.cc E:\my\game_svr_demo\proto\ /sy
REM xcopy Z:\test\game_svr_demo\dbproxy_svr\cpp_cfg\*.h E:\my\game_svr_demo\dbproxy_svr\cpp_cfg\ /sy

pause