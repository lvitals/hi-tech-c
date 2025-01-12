TARGET = dehuff
SRCS = dehuff.c decode.c misc.c fname.c getargs.c
HEADERS = huff.h

CC = gcc
CFLAGS = -Wall -Wextra -O3
CFLAGS_EXTRA = -DUSE_GETARGS

# Enable _getargs functionality for handling long command lines (necessary for legacy systems like CP/M)
# Uncomment the following line to include this support in the build
CFLAGS += $(CFLAGS_EXTRA)

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
