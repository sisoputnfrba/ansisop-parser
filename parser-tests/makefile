RM=rm -rf
CC=gcc
DIR=build
OUTPUT=test

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')

# Clean and compile .so
all:
	$(CC) $(C_SRCS) -o $(DIR)/$(OUTPUT) -lcommons -lparser-ansisop -lcspecs

build:
	mkdir -p $(DIR)
	make all

test:
	make build
	./$(DIR)/$(OUTPUT)

# Clean build files
clean:
	$(RM) $(DIR)

.PHONY: all clean build test
