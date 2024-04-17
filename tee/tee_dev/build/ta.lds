OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH("arm")
ENTRY(_start)
SECTIONS {
	.text : {
		*(.text .text.*)
		*(.stub)
		*(.glue_7)
		*(.glue_7t)
		*(.gnu.linkonce.t.*)
	}

	.plt : { *(.plt) }

	.eh_frame : { *(.eh_frame) }

	.rodata : {
		*(.gnu.linkonce.r.*)
		*(.rodata .rodata.*)
	}

	.ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) }

	.ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	.ctors : { *(.ctors) }
	.dtors : { *(.dtors) }
	.dynsym : { *(.dynsym) }
	.dynstr : { *(.dynstr) }
	.hash : { *(.hash) }

	/* Page align to allow dropping execute bit for RW data */
	. = ALIGN(4096);
	.data : { *(.data .data.* .gnu.linkonce.d.*) }

	.dynamic : { *(.dynamic) }
	.got : { *(.got.plt) *(.got) }
	.rel.text : { *(.rel.text) *(.rel.gnu.linkonce.t*) }
	.rela.text : { *(.rela.text) *(.rela.gnu.linkonce.t*) }
	.rel.data : { *(.rel.data) *(.rel.gnu.linkonce.d*) }
	.rela.data : { *(.rela.data) *(.rela.gnu.linkonce.d*) }
	.rel.rodata : { *(.rel.rodata) *(.rel.gnu.linkonce.r*) }
	.rela.rodata : { *(.rela.rodata) *(.rela.gnu.linkonce.r*) }
	.rel.dyn : { *(.rel.dyn) }
	.rel.got : { *(.rel.got) }
	.rela.got : { *(.rela.got) }
	.rel.ctors : { *(.rel.ctors) }
	.rela.ctors : { *(.rela.ctors) }
	.rel.dtors : { *(.rel.dtors) }
	.rela.dtors : { *(.rela.dtors) }
	.rel.init : { *(.rel.init) }
	.rela.init : { *(.rela.init) }
	.rel.fini : { *(.rel.fini) }
	.rela.fini : { *(.rela.fini) }
	.rel.plt : { *(.rel.plt) }
	.rela.plt : { *(.rela.plt) }

	.bss : {
		__bss_start__ = .;
		*(.bss .bss.* .gnu.linkonce.b.* COMMON)
		__bss_end__ = .;
	}
}
