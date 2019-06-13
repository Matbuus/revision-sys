BIN=exo-gdb

LDFLAGS=-pthread
CFLAGS=-O0

all: $(BIN)

exo-gdb: exo-gdb.c
	$(CC) exo-gdb.c -o exo-gdb $(CFLAGS) $(LDFLAGS)
clean:
	rm -f $(BIN)
