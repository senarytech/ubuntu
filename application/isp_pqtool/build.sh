#!/bin/bash
source build/header.rc
source build/install.rc
source build/module/toolchain/${CONFIG_TOOLCHAIN_APPLICATION}.rc
source build/chip.rc

PQ_TOOL_VERSION="3_2_6"

if [ "is${PQ_TOOL_VERSION}" = "is3_2_6" ]; then
server_path="./source/server/ctrl/3.2.6"
else
server_path="./source/server/ctrl/3.2"
fi


mod_dir=$(dirname "$0")
sysroot=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_SYSYROOT}
rootfs=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/${CONFIG_SYNA_SDK_OUT_ROOTFS}
opt_intermediate_dir=${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/isp_pqtool

mkdir -p ${opt_intermediate_dir}

[ ${clean} -eq 1 ] && make clean -C ${mod_dir} BUILD_DIR="${opt_intermediate_dir}" && exit 0

isp_cflags="-Wno-deprecated-declarations -fpermissive"
isp_cflags="${isp_cflags} -Wno-pedantic -Wno-error=unused-function"
isp_cflags="${isp_cflags} -std=c++11 -fpic -frtti -DISP_INPUT_MAX=1"
isp_cflags="${isp_cflags} -DEDITION_S1 -D__LINUX__"
if [ "is${PQ_TOOL_VERSION}" = "is3_2_6" ]; then
isp_cflags="${isp_cflags} -DCTRL_CNR -DCTRL_3DNR -DCTRL_IE -DCTRL_WDR3 -DCTRL_HTTP"
isp_cflags="${isp_cflags} -DCTRL_CAC -DCTRL_GC -DCTRL_SENSOR -DCTRL_BLS -DCTRL_2DNR -DCTRL_AWB"
isp_cflags="${isp_cflags} -DCTRL_DPCC -DCTRL_DPF -DCTRL_AF -DCTRL_HDR1_2 -DCTRL_EE -DCTRL_AE2"
isp_cflags="${isp_cflags} -DCTRL_LSC -DCTRL_DEMOSAIC -DCTRL_CPROC -DCTRL_WB -DCTRL_WDR"
else
isp_cflags="${isp_cflags} -DISP_CNR -DISP_2DNR -DISP_3DNR -DISP_IE -DISP_WDR_V3 -DISP_FILTER"
isp_cflags="${isp_cflags} -DPQ_TOOL_VERSION_3_2 -DGAIN_TABLE -DISP_EE"
#isp_cflags="${isp_cflags} -DISP_HDR_STITCH -DISP_HDR"
fi
isp_cflags="${isp_cflags} -DDATAPATH=\\\"/data/\\\""

isp_ldflags="${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}"
isp_ldflags="${isp_ldflags} -lboost_system -lampclient"
isp_ldflags="${isp_ldflags} -lcppnetlib-server-parsers -lcppnetlib-client-connections"

isp_incflags="-I${sysroot}/usr/include/amp"
if [ "is${PQ_TOOL_VERSION}" = "is3_2_6" ]; then
isp_incflags="${isp_incflags} -I${server_path}/tuning-server/include -I${server_path}/dependent"
isp_incflags="${isp_incflags} -I./source/interface"
else
isp_incflags="${isp_incflags} -I${server_path}/include -I${server_path}/dependent"
isp_incflags="${isp_incflags} -I./source/interface"
fi
if [ -z "${CONFIG_RUNTIME_RDK}" ];then
isp_incflags="${isp_incflags} -I${mod_dir}/../../external/jsoncpp/include"
isp_incflags="${isp_incflags} -I${mod_dir}/../../external/boost/"
isp_incflags="${isp_incflags} -I${mod_dir}/../../external/cppnetlib/"
fi

cflags=${CONFIG_TOOLCHAIN_APPLICATION_CFLAG}
cflags="${cflags} --sysroot=${sysroot}"

cc_name="${CONFIG_TOOLCHAIN_APPLICATION}gcc"
cxx_name="${CONFIG_TOOLCHAIN_APPLICATION}g++"

if [ -z "${CONFIG_RUNTIME_RDK}" ];then
isp_ldflags="${isp_ldflags} -L${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/boost/lib/"
isp_ldflags="${isp_ldflags} -L${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/cppnetlib/libs/network/src/"
isp_ldflags="${isp_ldflags} -L${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/jsoncpp/src/lib_json/"
fi

if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
    libtuning_path="${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/isp_pqtool"
    if [ "${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "clang" ]; then
        clangcflag=${CONFIG_TOOLCHAIN_CLANG_CFLAG}
        clangldflag=${CONFIG_TOOLCHAIN_CLANG_LDFLAG}
        cflags="${clangcflag} ${cflags}"
        isp_ldflags="${clangldflag} ${isp_ldflags}"
        cc_name="clang"
        cxx_name="clang++"
    else
        cflags="${cflags} -I${sysroot}/include/libcxx"
    fi
    isp_cflags="${isp_cflags} -DBOOST_ASIO_DISABLE_CXX11_ALLOCATORS -fPIE"
    isp_cflags="${isp_cflags} -DXMLPATH=\\\"/vendor/etc/\\\""
    isp_ldflags="${isp_ldflags} -pie -ltinyxml2 -lOSAL"
    jsonpath="${sysroot}/usr/lib/${ANDROID_SYSROOT_TARGET}/${CONFIG_RUNTIME_ANDROID_API_LEVEL}"
    jsonlib="${jsonpath}/libjsoncpp.a"
else
    if [ -z "${CONFIG_RUNTIME_RDK}" ];then
        libtuning_path="${CONFIG_SYNA_SDK_OUT_TARGET_PATH}/system/opt/syna/lib"
        isp_ldflags="${isp_ldflags} -ljsoncpp -ltinyxml2d"
    else
        libtuning_path="${sysroot}/usr/lib"
        isp_ldflags="${isp_ldflags} -ljsoncpp -ltinyxml2"
    fi
    isp_cflags="${isp_cflags} -DXMLPATH=\\\"/etc/\\\""
    jsonlib="${sysroot}/usr/lib/libjsoncpp.a"
fi

#Remove -fPIE to fix build error
#ld.lld: error: relocation R_ARM_REL32 cannot be used against symbol hFactoryAmpIsp;
#recompile with -fPIC
if [ "${CONFIG_TOOLCHAIN_APPLICATION_CC}" = "clang" ]; then
    isp_cflags=${isp_cflags/-fPIE/}
    cflags=${cflags/-fPIE/}
fi
# ------------------------
# Build libisppq.so
# ------------------------

if [[ ! -d ${mod_dir}/lib_rel ]]; then
    make all -C ${mod_dir}/${server_path} CC="${cc_name}" CXX="${cxx_name}" \
                E_CFLAG="${cflags} ${isp_cflags} ${isp_incflags}" \
                E_LDFLAG=" ${isp_ldflags} ${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG}" \
                BUILD_DIR="${libtuning_path}" STATIC_LIBS="${jsonlib}"
fi
# Add ld option for building isp_pqtool
if [[ -d ${mod_dir}/lib_rel ]]; then
    rel_lib_path="${mod_dir}/lib_rel/${syna_chip_name}${syna_chip_rev}"
    cp -f ${rel_lib_path}/${CONFIG_TOOLCHAIN_APPLICATION}/libisppq.so ${libtuning_path}/
fi

isp_ldflags="${isp_ldflags} -ldl -L ${libtuning_path} -lisppq"
if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
    isp_ldflags="${isp_ldflags} -lc++"
fi

# ------------------------
# Build isp_pqtool
# ------------------------
make all -C ${mod_dir} CC="${cc_name}" CXX="${cxx_name}" \
            E_CFLAG="${cflags} ${isp_cflags} ${isp_incflags}" \
            E_LDFLAG="${CONFIG_TOOLCHAIN_APPLICATION_LDFLAG} ${isp_ldflags}" \
            BUILD_DIR="${opt_intermediate_dir}" STATIC_LIBS="${jsonlib}"

mkdir -p ${rootfs}/usr/bin
make install -C ${mod_dir} APP_DIR="${rootfs}/usr/bin/" BUILD_DIR="${opt_intermediate_dir}"

# ------------------------
# Install for Android
# ------------------------
if [ "is${CONFIG_ANDROID_OS}" = "isy" ]; then
    libpath="/system/home/galois/lib/"
    binpath="/system/home/galois/bin/"
    INSTALL_F ${libtuning_path}/libisppq.so ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}${libpath}
    INSTALL_F ${rootfs}/usr/bin/isp_pqtool ${CONFIG_SYNA_SDK_OUT_TARGET_PATH}${binpath}
fi
