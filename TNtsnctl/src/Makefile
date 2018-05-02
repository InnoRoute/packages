#obj-m := TNtsnctl.o
#TNflowtable-objs	:= mastertable.o tnlibflowtable.o flowtableactions.o accdpactions.o tnlibaccdp.o
OBJ = tntsnctl.o tnlibtsnctl.o
all:
	$(MAKE)  -C ${KERNEL}  M=$(PWD) modules

tntsnctl: $(OBJ)

doku:
	indent -br -l150 *.c
	indent -br -l150 *.h
	doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean

