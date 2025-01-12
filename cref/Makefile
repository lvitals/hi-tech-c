TARGET = cref
SRCS = cref.c getargs.c fname.c

CC = gcc
CFLAGS = -Wall -Wextra -O3
CFLAGS_EXTRA = -DUSE_GETARGS

# Enable _getargs functionality for handling long command lines (necessary for legacy systems like CP/M)
# Uncomment the following line to include this support in the build
CFLAGS += $(CFLAGS_EXTRA)

OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
