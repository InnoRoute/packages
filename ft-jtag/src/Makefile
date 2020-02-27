CFLAGS = -Wall -O2 -s -Werror
LDFLAGS = -lusb -lftdi -s
PROG = ft-jtag

all:	$(PROG)

$(PROG):	$(PROG).cxx
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROG)

