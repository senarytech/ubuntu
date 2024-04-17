# mapping between module and subimg
preboot=preboot

key_1st=key
key_2nd=key
key_a=key
key_b=key
key=key

tzk_normal=tee
tzk_normalB=tee
tz_en=tee
tz_enB=tee
tz_en_a=tee
tz_en_b=tee

if [ -f ${outdir_subimg_intermediate}/tee_recovery.subimg ]; then
  tzk_recovery=tee_recovery
else
  echo "Warning: tzk recovery use the same tee binary with tzk normal"
  tzk_recovery=tee
fi
tzk_a=tee
tzk_b=tee
tzk=tee

if [ "is${CONFIG_UBOOT_BOOTUP_SYSTEM}" == "isy" ]; then
  bl_normal=fastboot
  bl_normalB=fastboot
  bl_recovery=fastboot
  bl_a=fastboot
  bl_b=fastboot
  bl=fastboot
  bootloader_a=fastboot
  bootloader_b=fastboot
  post_bootloader=fastboot
  postbootloaderB=fastboot
else
  bl_normal=bootloader
  bl_normalB=bootloader
  bl_recovery=bootloader
  bl_a=bootloader
  bl_b=bootloader
  bl=bootloader
  bootloader_a=bootloader
  bootloader_b=bootloader
  post_bootloader=bootloader
  postbootloaderB=bootloader
fi

boot=linux_bootimgs
bootimgs=linux_bootimgs
boot_a=linux_bootimgs
boot_b=linux_bootimgs
system=system
rootfs=system
rootfs_a=system
rootfs_b=system

fastboot_1st=fastboot
fastboot_2nd=fastboot
fastboot_a=fastboot
fastboot_b=fastboot
fastboot=fastboot

firmware=firmware
firmware_a=firmware
firmware_b=firmware

fastlogo=fastlogo
fastlogoB=fastlogo
fastlogo_a=fastlogo
fastlogo_b=fastlogo
