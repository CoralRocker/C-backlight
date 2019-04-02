CC=gcc

main.o : main.c
	$(CC) -c main.c

install: main.o
	sudo $(CC) -o /bin/blight main.o
	rm main.o
  
compile-only: main.o
	$(CC) -o blight main.o
	rm main.o
