OBJEXT = .o
BINEXT =
CC = gcc
CFLAGS = -g -Wall -std=c89

cpm: cpm$(OBJEXT)
	$(CC) $(LIBS) cpm$(OBJEXT) -o cpm$(BINEXT)

cpm$(OBJEXT): cpm.c
	$(CC) -c $(CFLAGS) cpm.c -o cpm$(OBJEXT)

clean:
	$(RM) cpm$(OBJEXT) cpm$(BINEXT)
