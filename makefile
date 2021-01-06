all: saltpass.c
	gcc -o sp saltpass.c -static -lcrypto
