CFLAGS = -Wall -O2

OBJS = led.o hid.o

all: libled.a

libled.a: $(OBJS)
	$(AR) rv $@ $(OBJS)

clean:
	rm -f *.o libled.a
