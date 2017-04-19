RM=rm -rf
CC=gcc -std=c99

C_SPEC=../cspec
C_SPEC_BIN=$(C_SPEC)/release

COMMONS=../commons/src
COMMONS_BIN=$(COMMONS)/build

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=build/%.o)

all: build/test

create-dirs:
	mkdir -p build

build/test: dependents create-dirs $(OBJS)
	$(CC) -L"../parser/build" -L"$(C_SPEC_BIN)" -L"$(COMMONS_BIN)" -o "build/ansisop-tests" $(OBJS) -lcommons -lparser-ansisop -lcspecs

build/%.o: ./%.c
	$(CC) -I"../parser" -I"$(C_SPEC)" -I"$(COMMONS)" -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

debug: CC += -DDEBUG -g
debug: all

clean:
	$(RM) build

test: all
	LD_LIBRARY_PATH="../parser/build/:$(C_SPEC_BIN):$(COMMONS_BIN)" build/ansisop-tests

dependents: install-cspec install-commons
	-cd ../parser/ && $(MAKE) all

install-commons:
	-cd ../ && git submodule init && git submodule update
	-cd ../commons && $(MAKE) all

install-cspec:
	-cd ../ && git submodule init && git submodule update
	-cd ../cspec && $(MAKE) all

.PHONY: all create-dirs clean install-cspec install-commons
