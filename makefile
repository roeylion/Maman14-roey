CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

# Source files
SRCS = assembler.c pre_processor.c helpers.c globals.c binary.c errors.c parser.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = pre_processor.h helpers.h globals.h binary.h errors.h parser.h

# Executable
TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)
