CC=gcc

FLAGS=-O3 -Wno-unused-result
LDFLAGS=-pthread
#DEBUG=-DDEBUG
RESULT=-DRESULT

all: golB

gol: golB.c
	$(CC) $(DEBUG) $(RESULT) $(FLAGS) golB.c -o golB

clean:
	rm -rf golB
