#DVB_DIR = $(HOME)/hg/v4l-dvb/linux

obj-m = dvbloopback.o
dvbloopback-objs := dvb_loopback.o dvblb_proc.o dvblb_forward.o

ifdef DVB_DIR
  EXTRA_CFLAGS = -I$(SOURCE_DIR) -I$(SOURCE_DIR)/include \
     -I$(SOURCE_DIR)/include/uapi -I$(SOURCE_DIR)/arch/x86/include \
     -I$(DVB_DIR)/drivers/media/dvb-core \
     -I$(DVB_DIR)/drivers/media/dvb/dvb-core
  DVB_SRC = $(DVB_DIR)
  SYMVER = /lib/modules/$(shell uname -r)/build/Module.symvers
  have_modver := $(wildcard $(SYMVER))
  BUILD_DIR ?= /lib/modules/$(shell uname -r)/build
  all: add_modver
	./config_dvb_dvbdir.pl "BUILD_DIR=$(BUILD_DIR)" "EXTRA_CFLAGS=$(EXTRA_CFLAGS)"
	make -C $(BUILD_DIR) M=$(PWD) modules
else
  EXTRA_CFLAGS += -Idrivers/media/dvb-core/ -I$(PWD) \
                  -Idrivers/media/dvb/dvb-core/
  SYMVER = /lib/modules/$(shell uname -r)/build/Module.symvers
  have_modver := $(wildcard $(SYMVER))
  BUILD_DIR ?= /lib/modules/$(shell uname -r)/build
  all: add_modver
	./config_dvb.pl "BUILD_DIR=$(BUILD_DIR)" "EXTRA_CFLAGS=$(EXTRA_CFLAGS)"
	make -C $(BUILD_DIR) M=$(PWD) modules
endif

ifeq ($(strip $(have_modver)),) 
add_modver:
	echo "Skipping Modver $(SYMVER)"
else
add_modver: $(SYMVER)
	cp -f $(SYMVER) .
endif

clean:
	make -C $(BUILD_DIR) M=$(PWD) clean
	cd config-dvb && make clean
	rm -f config-dvb/chkdvb.c
	rm -f dvbdevwrap.h
	rm -f dvbdev.h
	rm -f Module.symvers

