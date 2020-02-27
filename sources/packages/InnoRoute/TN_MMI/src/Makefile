obj-m := INR_TN_MMI.o
INR_TN_MMI-objs	:= main.o TN_MMI.o TN_MDIO.o

all:
	$(MAKE)  -C ${KERNEL}  M=$(PWD) modules

SRC := $(shell pwd)
bb:
	$(MAKE) -C $(KERNEL_PATH) M=$(SRC)

doku:
	astyle *.c
	astyle *.h
#	indent -br -l150 *.c
#	indent -br -l150 *.h
	doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean
	
modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

