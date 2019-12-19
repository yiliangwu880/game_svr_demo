#!/bin/sh
# 根据配置，导出解析用h文件。

#login
cp ../external/cpp_cfg/bin/export_cpp_cfg ../bin/login1/
cd ../bin/login1
./export_cpp_cfg cfg.txt
cd -
mv ../bin/login1/*.h ../login/

#statistics
cp ../external/cpp_cfg/bin/export_cpp_cfg ../bin/statistics1/
cd ../bin/statistics1
./export_cpp_cfg cfg.txt
cd -
mv ../bin/statistics1/*.h ../statistics/

#team
cp ../external/cpp_cfg/bin/export_cpp_cfg ../bin/team1/
cd ../bin/team1
./export_cpp_cfg cfg.txt
cd -
mv ../bin/team1/*.h ../team/

#zone
cp ../external/cpp_cfg/bin/export_cpp_cfg ../bin/zone1/
cd ../bin/zone1
./export_cpp_cfg cfg.txt
cd -
mv ../bin/zone1/*.h ../zone/


#simulate_client
cp ../external/cpp_cfg/bin/export_cpp_cfg ../bin/simulate_client1/
cd ../bin/simulate_client1
./export_cpp_cfg cfg.txt
cd -
mv ../bin/simulate_client1/*.h ../simulate_client/