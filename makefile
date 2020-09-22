all: saltpass.c
	gcc -o saltpass saltpass.c -static -lcrypto
