简单说，一共三个步骤：


0.配置本地docker 环境  （因为windows 文件系统问题，不建议在windows 系统的文件配置到docker编译环境使用，避免发生一些奇怪的问题,以下操作都没有在windows 下面测试过。)
1.创建一个标准ubuntu 20.04的镜像
3.切换到代码目录，开始编译。


1.解压压缩到本地，并切换到解压后的目录，使用下面命令，创建docker image 。

docker build -t ubuntussh:1.00  .     (注意这个空格加. 表示在当前目录查找docker file) 
   
出现下面的输出，就表示成功：
 => => naming to docker.io/library/ubuntussh:1.00

2.运行。
docker run -d -p 8888:22 -v /home/user/code:/home/code   --privileged=true  ubuntussh:1.00 
其中8888 ，表示端口转发，本地8888 端口转发到docker 的22 端口，用来给ssh 用，-v参数带入本地目录到docker 目录，：前是本地的绝对目录地址，后面是docker 目录，已经在docker 文件里面创建 /home/code. 最后的名称需要和第一步匹配上。
第一步只需要做一次。这步可以反复执行，每个开发，都可以执行，创建自己的docker 。每个人的端口需要独立，不能够重复。

3.本地ssh 链接上去，并运行命令：
ssh root@127.0.0.1 -p 8888
password:root

4.进入目录，进行编译。
 cd /home/code
 改变当前目录到代码根目录。
 make vs680_a0_ubuntu20_evk_defconfig && make
 
 备注：因为网络的问题，可能有个别组件安装失败。需要手工安装下。以上步骤在ubuntu20.04测试通过。
 

