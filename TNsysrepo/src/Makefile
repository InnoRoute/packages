OBJ = tnsysrepo_main.o \
	tnsysrepo.o \
	tnsysrepo_flowcache.o \
	tnsysrepo_tas.o \
	TAS/tnlibtsnctl.o \
	Flowcache/mastertable.o \
	Flowcache/flowtableactions.o \
	Flowcache/tnlibflowtable.o \
	Flowcache/tnlibaccdp.o \
	Flowcache/accdpactions.o 


tnsysrepo_main: $(OBJ)
	$(CC) $(LDFLAGS) -o tnsysrepo_main $(notdir $(OBJ)) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

doku:
	indent -br -l150 *.c
	indent -br -l150 *.h
	cd .. && doxygen Doxyfile
	
clean:
	$(MAKE) -C ${KERNEL}  M=$(PWD) clean

