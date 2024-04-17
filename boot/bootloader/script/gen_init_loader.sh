WORKING_DIR=`pwd`
SDK_PATH=${WORKING_DIR%%MV88DE3100_SDK*}
PREBUILTS_PATH="$SDK_PATH"prebuilts
PREBUILTS_PATH2="$SDK_PATH"../prebuilts/gcc/linux-x86/aarch64/gcc-linaro-aarch64-elf-5.3/bin
MRVL_PATHS=$PREBUILTS_PATH/gcc/aarch64-gcc-5.3/gcc-aarch64-linux-gnu/bin:$PREBUILTS_PATH/gcc/aarch64-gcc-5.3/gcc-aarch64-elf/bin:$PREBUILTS_PATH/gcc/arm-marvell-eabi/bin
export PATH=$PREBUILTS_PATH2:$MRVL_PATHS:$PATH


if [ "$1" = "post_bl" ]; then
	./script/gen_bl_img2.sh
else
source ../../../Customization_Data/config
export CONFIG_BCM
export BERLIN_CHIP

if [ "$CONFIG_BCM" = "y" ]; then
	CHIP_NAME=`echo $BERLIN_CHIP | tr '[A-Z]' '[a-z]'`
	if [ "$CHIP_NAME" = "bg3cda0" ]; then
		./script/gen_init_loader_bg3cda0.sh $1
	else
		./script/gen_bl_img1.sh $1
	fi
else
./script/gen_init_loader_drm.sh
fi
fi
