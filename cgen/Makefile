CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -std=c89
# ifeq ($(shell uname), Linux)
#   CFLAGS += -Wno-implicit-fallthrough -Wno-stringop-truncation -Wno-stringop-overflow
# endif
SRCS = code.c data.c getchar.c lex.c main.c getargs.c fname.c
OBJS = $(SRCS:.c=.o)

HEADERS = cgen.h
TARGET = cgen

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
