TARGET = zas
SRCS = cclass.c code.c expr.c fname.c kwd.c lex.c macro.c parse.c sym.c tlab.c zas.c
HEADERS = cclass.h kwd.h zas.h

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c89

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
