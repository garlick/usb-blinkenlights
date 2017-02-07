CFLAGS = -Wall -O2
LIBADD = -lusb
OBJS = renderchar.o led.o hid.o

ALL = renderchar

all: $(ALL)

renderchar: $(OBJS)
	$(CC) -o $@  $(OBJS) $(LIBADD)

clean:
	rm -f *.o $(ALL)
