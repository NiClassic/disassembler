CC = gcc
CFLAGS = -Wall
EXEC = disassembler
SRCS = disassembler.c instruction_types.c

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRCS)

clean:
	rm -f $(EXEC)

.PHONY: all clean
