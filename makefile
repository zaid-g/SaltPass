all: saltpass.c
	gcc -o saltpass saltpass.c -lssl -lcrypto
