TARGET = link
SRCS = extra.c io.c lib.c link.c misc.c reloc.c sym.c text.c
HEADERS = link.h

CC = gcc
CFLAGS = -Wall -Wextra -std=c89

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
