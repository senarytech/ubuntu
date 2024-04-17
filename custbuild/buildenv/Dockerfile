FROM ubuntu:20.04 

ENV TZ Asia/Shanghai
ENV LANG zh_CN.UTF-8
RUN echo 'root:root' |chpasswd
RUN echo "deb http://mirrors.163.com/ubuntu/ focal main restricted universe multiverse\n \
deb http://mirrors.163.com/ubuntu/ focal-security main restricted universe multiverse \n \
deb http://mirrors.163.com/ubuntu/ focal-updates main restricted universe multiverse\n	\
deb http://mirrors.163.com/ubuntu/ focal-backports main restricted universe multiverse\n"\
            > /etc/apt/sources.list        \
            && apt update && apt install -y \ 
			openssh-server \
			vim \
			&& apt clean \
			&& rm -rf /tmp/* /var/lib/apt/lists/* /var/tmp* \
			&& echo "PermitRootLogin yes" >> /etc/ssh/sshd_config
RUN mkdir /var/run/sshd

RUN mkdir /home/code && mkdir /home/tools

RUN apt-get update && apt-get install -y cmake rsync
RUN apt-get install -y doxygen coreutils fakeroot squashfs-tools  cryptsetup-bin git gnupg flex bison build-essential zip curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 lib32ncurses5-dev x11proto-core-dev
RUN apt-get install -y libx11-dev lib32z1-dev  unzip  libssl-dev bc python-is-python2 python3 python3-pip python3-pexpect python3-markdown gawk  git diffstat texinfo build-essential chrpath socat cpio
RUN apt-get install -y  debianutils iputils-ping xterm libmicrohttpd-dev libargtable2-dev  ninja-build gzip liblz4-tool ffmpeg automake genext2fs openjdk-8-jdk gcc-aarch64-linux-gnu g++-aarch64-linux-gnu gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf   

RUN apt-get install -y  flex bison lz4 ninja-build  kmod gdisk dosfstools
RUN pip install autopep8==1.2.1 

#RUN apt-get install 	perf z-utils libI1-mesa-dev libxmI2-utils xsitproc repo x-utils libsdi1.2-dev clang-format-3.8 cc realpath cache

COPY cmake-3.25.2-linux-x86_64.tar.gz  /home/tools
RUN  cd /home/tools/ && tar -xf cmake-3.25.2-linux-x86_64.tar.gz  && cp cmake-3.25.2-linux-x86_64/bin/* /usr/bin/ && cp cmake-3.25.2-linux-x86_64/share/* /usr/share/ -r  && rm cmake-3.25.2-linux-x86_64 -fr


EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]


