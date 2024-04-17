#!/bin/bash

apt update

sudo apt-get install -y cache doxygen cc coreutils fakeroot squashfs-tools 
sudo apt-get install -y realpath liblz4-tool z-utils cryptsetup-bin
sudo apt-get install -y git-core gnupg flex bison perf build-essential zip
sudo apt-get install -y curl zlib1g-dev gcc-multilib g++-multilib libc6-dev-i386 
sudo apt-get install -y lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z-dev 
sudo apt-get install -y libI1-mesa-dev libxmI2-utils xsitproc unzip repo 
sudo apt-get install -y libssl-dev bc
sudo apt-get install -y python python3 python3-pip python3-pexpect python3-markdown 
sudo apt-get install -y gawk wet git-core diffstat texinfo build-essential chrpath socat cpio
sudo apt-get install -y x-utils debianutils iputils-ping libsdi1.2-dev xterm 
sudo apt-get install -y libmicrohttpd-dev libargtable2-dev clang-format-3.8 ninja-build
sudo apt-get install -y gzip ffmpeg flex bison cmake
sudo pip install autopep8==1.2.1 -upgrade
#wget https://cmake.org/files/V3.6/cmake-3.6.0-Linux-x86_64.sh
#chmod a+ cmake-3.6.0-Linux-×86_64.sh
#sudo /cmake-3.6.0-Linuxx86_64.sh -prefix=/usr/ exclude subdir

#sudo apt install -y cmake   3.19 or hi.

sudo apt install -y wget cmake rsync flex bison lz4 ninja-build kmod gdisk dosfstools

wget https://cmake.org/files/LatestRelease/cmake-3.25.2-linux-x86_64.tar.gz
tar -xf cmake-3.25.2-linux-x86_64.tar.gz
sudo cp cmake-3.25.2-linux-x86_64/bin/* /usr/bin/
sudo cp cmake-3.25.2-linux-x86_64/share/* /usr/share/ -r
cmake -version

sudo apt install -y automake 
sudo apt-get install -y genext2fs

sudo apt-get update
sudo apt-get install -y openjdk-8-jdk
sudo update-alternatives -config java
sudo update-alternatives -config javac


#aarch64 gnu tools:
sudo apt install -y gcc-aarch64-linux-gnu
sudo apt install -y g++-aarch64-linux-gnu
sudo apt install -y gcc-arm-linux-gnueabihf
sudo apt install -y g++-arm-linux-gnueabihf
