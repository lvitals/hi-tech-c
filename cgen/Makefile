CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = cgen.c
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
