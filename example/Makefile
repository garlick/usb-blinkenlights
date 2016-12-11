CFLAGS = -Wall -O2 -I../libled
LIBADD = -L../libled -lled -lusb

ALL = renderchar

all: $(ALL)

renderchar: renderchar.o
	$(CC) -o $@ $< $(LIBADD)

clean:
	rm -f *.o $(ALL)

