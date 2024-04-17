TOP=$(PWD)
HIDE=@
VER_NUM=1.1.6
ALSALIB=alsa-lib-$(VER_NUM)
ALSAUTIL=alsa-utils-$(VER_NUM)
export PATH := $(PWD)/../../toolchain/aarch32/armv7a-cros-linux-gnueabi-4.9/bin:$(PATH)

DESTDIR=/home/galois

HOST=armv7a-cros-linux-gnueabi
TARGET=armv7a-cros-linux-gnueabi
CC=armv7a-cros-linux-gnueabi-gcc

.PHONY: all clean alsa alsa-lib alsa-utils
all: alsa

# alsa player
alsa: alsa-lib alsa-utils

alsa-lib:
	cd $(TOP)/$(ALSALIB); ./configure \
		--prefix=$(DESTDIR) \
		--host=$(HOST) \
		--target=$(TARGET) \
		--disable-python
	make -C $(TOP)/$(ALSALIB)
	make -C $(TOP)/$(ALSALIB) install

alsa-utils: alsa-lib
	cd $(TOP)/$(ALSAUTIL); ./configure \
		--prefix=$(DESTDIR) \
		--host=$(HOST) \
		--target=$(TARGET) \
		--with-alsa-inc-prefix=$(DESTDIR)/include \
		--with-alsa-prefix=$(DESTDIR)/lib \
		--with-udev-rules-dir=$(DESTDIR)/lib \
		--disable-alsaconf \
		--disable-alsamixer \
		--disable-xmlto \
		--disable-nls
	make -C $(TOP)/$(ALSAUTIL)
	make -C $(TOP)/$(ALSAUTIL) install

clean:
	rm -rf $(TOP)/install
	-make -C $(TOP)/$(ALSALIB) clean
	-make -C $(TOP)/$(ALSAUTIL) clean
