DEBUG=-g -D_DEBUG
#DEBUG=
#preprocessor (common to C and C++)
CPPFLAGS=$(DEBUG) -W -Wall -Wextra -Werror -ansi -pedantic -pedantic-errors -Winvalid-pch -fpch-preprocess
#C
CC=gcc
CFLAGS=-std=c99
#C++
CXX=g++
CXXFLAGS=-Woverloaded-virtual -std=gnu++98 -Wno-long-long
CXXFLAGS+=-Wpedantic-ms-format
CXXFLAGS+=-std=c++98
#CXXFLAGS+=-std=c++0x
#ar
AR=ar
#ranlib
RANLIB=ranlib
#link (common to C and C++)
LD=ld
LDFLAGS=-Wl,--reduce-memory-overheads -Wl,--no-keep-memory -Wl,--as-needed
LOADLIBES=
LDLIBS=

PROGRAM=code_of_duty

#disable built-in implicit rules
.SUFFIXES:
#disable automatic deletion of intermediate files
.SECONDARY:
#delete output file if error occured
.DELETE_ON_ERROR:

all: $(PROGRAM).exe

clean:
	@-rm $(PROGRAM).o 2>/dev/null

distclean: clean
	@-rm $(PROGRAM).exe 2>/dev/null

mrproper: distclean

.PHONY: all clean distclean mrproper

#generic rules

%.exe: %.cc.o
	$(CXX) -o $@ $(filter %.o,$^) $(LDFLAGS) $(LOADLIBES) $(LDLIBS)

%.exe: %.o
	$(CC) -o $@ $(filter %.o,$^) $(LDFLAGS) $(LOADLIBES) $(LDLIBS)

%.cc.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -o $@ -c $(filter %.cpp,$^)

%.cc.o: %.cxx
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -o $@ -c $(filter %.cxx,$^)

%.cc.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) -o $@ -c $(filter %.cc,$^)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDES) -o $@ -c $(filter %.c,$^)
