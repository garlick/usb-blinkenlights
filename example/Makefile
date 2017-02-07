CFLAGS = -Wall -O2
LIBADD = -lusb
OBJS = led.o libs-host/opendevice.o

ALL = led

all: $(ALL)

led: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBADD)

clean:
	rm -f *.o libs-host/*.o $(ALL)
