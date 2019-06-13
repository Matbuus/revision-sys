BIN=client serveur

CFLAGS=-Wall
LDFLAGS=-pthread

all:$(BIN)

client: client.c client-serveur.h
	$(CC) client.c -o client $(CFLAGS) $(LDFLAGS)

serveur: serveur.c client-serveur.h
	$(CC) serveur.c -o serveur $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(BIN) tube_*
