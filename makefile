
.PHONY: clean

.SUFFIXES:

DEBUG = yes

CC = gcc
CXX = g++

PATH_INC = -I/usr/include -I/usr/local/include
PATH_LIB = -L/usr/local/lib -L./lib

LIBS = -lwiringPi -lmatrix_creator_hal -lsnowboy-detect -lcblas -lpthread

ifeq ($(DEBUG),yes)
	CFLAGS = -g -W -Wall
	CXXFLAGS = -g -W -Wall
else
	CFLAGS = -W -Wall
	CXXFLAGS = -W -Wall
endif

all: leds mics

mics: mics.o snowboy_wrapper.o
	$(CXX) $^ -o $@ $(CXXFLAGS) $(PATH_LIB) $(LIBS)

leds: leds.o 
	$(CXX) $^ -o $@ $(CXXFLAGS) $(PATH_LIB) $(LIBS)

%.o: %.cpp
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

%.o: %.cc
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

clean:
	rm -rf *.o *.bak mics leds
