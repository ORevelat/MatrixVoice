
.PHONY: clean

.SUFFIXES:

DEBUG = yes

CC = gcc
CXX = g++

PATH_INC = -I/usr/include -I/usr/local/include
PATH_LIB = -L/usr/local/lib -L./lib

LIBS = -lwiringPi -lmatrix_creator_hal -lsnowboy-detect -lcblas -lpthread -lcurl -lgflags -lglog

ifeq ($(DEBUG),yes)
	CFLAGS = -g -W -Wall
	CXXFLAGS = -g -W -Wall
else
	CFLAGS = -W -Wall
	CXXFLAGS = -W -Wall
endif

all: sarah

sarah: main.o http_server.o http_client.o speak.o listen.o matrix_leds.o matrix_mics.o snowboy_wrapper.o
	$(CXX) $^ -o $@ $(CXXFLAGS) $(PATH_LIB) $(LIBS)

%.o: %.cpp %.h
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

%.o: %.cc  %.h
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

%.o: %.c  %.h
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

clean:
	rm -rf a.out *.o *.bak sarah
