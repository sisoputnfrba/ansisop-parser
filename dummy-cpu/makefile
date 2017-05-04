RM=rm -rf
CC=gcc

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=build/%.o)

all: build/dummy-cpu

create-dirs:
	mkdir -p build/.

build/dummy-cpu: dependents create-dirs $(OBJS)
	$(CC) -Wall -L"../parser/build" -L"../commons/src/build" -o "build/dummy-cpu" $(OBJS) -lparser-ansisop -lcommons

build/%.o: ./%.c
	$(CC) -Wall -I"../parser" -I"../commons/src" -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

debug: CC += -DDEBUG -g
debug: all

clean:
	$(RM) build

dependents: install-commons
	-cd ../parser/ && $(MAKE) all

install-commons:
	-cd ../ && git submodule init && git submodule update
	-cd ../commons && $(MAKE) all

.PHONY: all create-dirs clean install-commons
