#启动所有服务器进程
#命令说明：
#./run.sh stop   --停掉所有进程
#./run.sh clearLog   --清日志



user_name=`whoami`

#$1 进程关键字，会用来grep
function KillProcess(){
    echo "KillProcess $1"
	ps -ef|grep $user_name|grep -v "grep"|grep -v $0|grep $1|awk '{print $2}' |xargs kill -USR1 &>/dev/null
	
}

#关闭一个进程
#$1 进程关键字，会用来grep
function KillOneProcess(){
    echo "KillProcess $1"
	ps -ef|grep $user_name|grep -v "grep"|grep -v $0|grep $1|awk '{print $2}' | head -n 1|xargs kill -USR1 &>/dev/null
	
}

function StartServer(){
cd acc1
./acc_svr
cd -
cd acc2
./acc_svr
cd -

cd msg_forward1
./mf_svr
cd -
cd msg_forward2
./mf_svr
cd -
sleep 1s 

cd statistics1
./statistics
cd -

sleep 1s 

cd zone1
./zone
cd -
#cd zone2
#./zone
#cd -
#cd zone3
#./zone
#cd -

cd team1
./team
cd -

cd login1
./login
cd -
#cd login2
#./login
#cd -

sleep 1s 
}

function StartClient(){

cd simulate_client1
./simulate_client
cd -
#cd simulate_client2
#./simulate_client
#cd -

}
function SimpleTest(){
cd acc1
./acc_svr
cd -

cd msg_forward1
./mf_svr
cd -
sleep 1s 

cd statistics1
./statistics
cd -

sleep 1s 

cd zone1
./zone
cd -

cd team1
./team
cd -

cd login1
./login
cd -


sleep 1s 

cd simulate_client1
./simulate_client
cd -
}

function clearLog(){
	find . -name svr_util_log* | xargs -i rm {}
}

function stop(){
	KillProcess "acc_svr"
	KillProcess "mf_svr"
	KillProcess "statistics"
	KillProcess "zone"
	KillProcess "team"
	KillProcess "login"
	KillProcess "simulate_client"
}

#------------------main -------------------
#Init
if [ $# -lt 1 ];then
	echo "run all"
	StartServer
	StartClient
else
    echo "run submodue" $1
	$1
fi

