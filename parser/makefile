RM=rm -rf
CC=gcc

C_SRCS=$(shell find . -iname "*.c" | tr '\n' ' ')
H_SRCS=$(shell find . -iname "*.h" | tr '\n' ' ')
OBJS=$(C_SRCS:./%.c=build/%.o)

# Clean and compile .so
all: build/libparser-ansisop.so

create-dirs:
	mkdir -p build/parser

build/libparser-ansisop.so: create-dirs $(OBJS)
	$(CC) -L"../commons/src/build" -shared -Wall -o "build/libparser-ansisop.so" $(OBJS) -lcommons

build/parser/%.o: parser/%.c
	$(CC) -I"../commons/src" -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

# Add debug parameters and compile
debug: CC += -DDEBUG -g
debug: all

# Clean build files
clean:
	$(RM) build

dependents:
	-cd ../commons/ && $(MAKE) all

install: all
	cp -u build/libparser-ansisop.so /usr/lib
	cp --parents -u $(H_SRCS) /usr/include

uninstall:
	rm -f /usr/lib/libparser-ansisop.so
	rm -rf /usr/include/parser

.PHONY: all create-dirs clean install uninstall
