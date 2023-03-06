CC=gcc
FLAGS=-Wall -Wextra -std=c11 -pedantic

conversor: main.c
	$(CC) $(CFLAGS) -o conversor main.c