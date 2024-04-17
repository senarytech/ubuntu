How to download SDK ？

git clone git@github.com:senarytech/ubuntu.git --recursive

How to compile ？

Get the toolchain and put it to ubuntu dir

Prepare the docker environment,

Firstly unzip sysroot/sysroot.tar.bz2 under ubuntu dir,

then please read custbuild/buildenv/readme

after build docker environment,you can enter custbuild dir

./build_evk.sh,


Burn the Image

ater build success, you can get image from out/vs680_a0_ubuntu20_evk/target/release/eMMCimg/
