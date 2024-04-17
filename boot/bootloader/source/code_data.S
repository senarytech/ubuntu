#ifdef CONFIG_FASTLOGO
#if defined(CONFIG_TRUSTZONE) && defined(CONFIG_MANUAL_TA_LOADING)
	.align			2
	.global			__fastlogo_ta_start
__fastlogo_ta_start:
	.word           (. + 4)
	.incbin         "libfastlogo.ta"
	.align			2
	.globl          __fastlogo_ta_end
	.word           (. + 4)
__fastlogo_ta_end:
	.word
#endif
#endif
#ifdef CONFIG_SM
	.align		2
	.globl		sm_start
sm_start:
	.word		(. + 4)
	.incbin		"sm.bin"
#if SM_ROM_ENABLE == 1
	.globl		sm_rom_start
sm_rom_start:
	.word		(. + 4)
	.incbin		"sm.rom.bin"
#endif
	.globl		sm_end
sm_end:
	.word		.
#endif
#ifdef BOOTIMG_ENCRYPT_DISABLE
#ifdef CONFIG_DTB
	.align          2
	.globl          dtb_start
dtb_start:
	.word           (. + 4)
	.incbin         "dtb.dtb"
	.globl          dtb_end
dtb_end:
	.word           .
#endif
#endif
