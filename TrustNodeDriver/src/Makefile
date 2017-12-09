obj-m := trustnode.o
trustnode-objs	:= main.o INR.o INR-NW.o INR-PCI.o INR-ctl.o INR-NAPI.o INR-MMI.o

all:
	$(MAKE)  -C ${KERNEL}  M=$(PWD) modules

doku:
	astyle *.c
	astyle *.h
#	indent -br -l150 *.c
#	indent -br -l150 *.h
	doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean

