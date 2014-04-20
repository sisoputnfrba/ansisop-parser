RM=rm -rf
CC=gcc

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=build/%.o)

all: build/so-ansisop-parser-dummy

create-dirs:
	mkdir -p build/.

build/so-ansisop-parser-dummy: dependents create-dirs $(OBJS)
	$(CC) -Wall -L"../parser/build" -o "build/so-ansisop-parser-dummy" $(OBJS) -lparser-ansisop -lcommons

build/%.o: ./%.c
	$(CC) -I"../parser" -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

debug: CC += -DDEBUG -g
debug: all

clean:
	$(RM) build

dependents:
	-cd ../parser/ && $(MAKE) all

.PHONY: all create-dirs clean
