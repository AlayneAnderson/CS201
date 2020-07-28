# NOTE: All indented lines are indented with a tab character.

CFLAGS = -ansi -pedantic -O0 -Wall
DFLAGS = -g
PROGS= typing_word_game

all: typing_word_game

typing_word_game: typing_word_game.c
	gcc typing_word_game.c $(CFLAGS) $(DFLAGS) -o typing_word_game

clean:
	rm -f $(PROGS) *.o typing_word_game

zip:
	zip rename_me.zip Makefile typing_word_game.c typescript