# Makefile to compile all .c files in current directory

# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Wextra -O2

# Find all .c files in current directory
SRC := $(wildcard ./*.c)

# Generate executable names by stripping .c
EXE := $(patsubst %.c,%,$(SRC))

# Default target
all: $(EXE)

# Rule to compile each .c into its executable
%: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up generated executables
clean:
	rm -f $(EXE)

.PHONY: all clean
