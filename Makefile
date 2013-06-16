CXX=clang++
CC=clang++
CXXFLAGS=-O3 -ansi -MD -Wall -Werror -Wno-sign-compare -std=c++11 -DDISABLE_LOG
LDFLAGS=-pthread
SOURCES:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SOURCES))

.PHONY: all clean rebuild

all: jcore

clean:
	rm -f *.o *.d jcore

rebuild: clean all

jcore: $(OBJS)
