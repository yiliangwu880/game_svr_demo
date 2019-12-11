简介：
	演示验证所有RS里面所有服务器组件
	
编译：
	整个文件夹放到linux目录，安装cmake gcc git等。

	git submodule init      --更新子模块
	git submodule update			
	git checkout -f	master		--强制删掉本地分支， track远程master分支
	去目录 external里面，参考说明编译每个文件夹，生成依赖库。
	主目录执行：sh clearBuild.sh 完成编译	
	
详细说明参考：说明.docx