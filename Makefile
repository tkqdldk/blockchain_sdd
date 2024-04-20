CFLAGS = -Wall
CC = gcc

PROGRAMS = run_signature_main run_main run_rsa_main run_decentralise_main run_block_main

all: $(PROGRAMS)

run_block_main: protocoleRSA.o primalite.o declarations.o decentralise.o block.o block_main.o
	$(CC) -o $@ $(CFLAGS) -lssl -lcrypto $^ -lm

run_decentralise_main: protocoleRSA.o primalite.o declarations.o decentralise.o decentralise_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

run_signature_main: protocoleRSA.o primalite.o declarations.o declarations_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

run_rsa_main: protocoleRSA.o primalite.o protocoleRSA_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

run_main: primalite.o primalite_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

primalite_main.o: primalite_main.c
	$(CC) -c $(CFLAGS) primalite_main.c

protocoleRSA_main.o: protocoleRSA_main.c
	$(CC) -c $(CFLAGS) protocoleRSA_main.c

declarations_main.o: declarations_main.c
	$(CC) -c $(CFLAGS) declarations_main.c

decentralise_main.o: decentralise_main.c
	$(CC) -c $(CFLAGS) decentralise_main.c

block_main.o: block_main.c
	$(CC) -c $(CFLAGS) -lssl -lcrypto block_main.c

procotoleRSA.o: protocoleRSA.c
	$(CC) -c $(CFLAGS) protocoleRSA.c

primalite.o: primalite.c
	$(CC) -c $(CFLAGS) primalite.c

declarations.o: declarations.c
	$(CC) -c $(CFLAGS) declarations.c

decentralise.o: decentralise.c
	$(CC) -c $(CFLAGS) decentralise.c

block.o: block.c
	$(CC) -c $(CFLAGS) -lssl -lcrypto block.c

clean:
	rm -f *.o *~ $(PROGRAMS)
	rm -f *.txt