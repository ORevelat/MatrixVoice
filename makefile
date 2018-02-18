
RECORD = no
DEBUG = no

CC = gcc
CXX = g++

PATH_INC = -I/usr/include -I/usr/local/include
PATH_LIB = -L/usr/local/lib -L./lib

LIBS = -lwiringPi -lmatrix_creator_hal -lsnowboy-detect -lcblas -lpthread -lcurl -lgflags -lglog

EXE = sarah

ifeq ($(DEBUG),yes)
	CFLAGS = -g -W -Wall
	CXXFLAGS = -g -W -Wall
else
	CFLAGS = -W -Wall
	CXXFLAGS = -W -Wall
endif

ifeq ($(RECORD),yes)
	CFLAGS += -DRECORD_TOFILE=1
	CXXFLAGS += -DRECORD_TOFILE=1
	EXE = sarah-record
endif

all: $(EXE)

$(EXE): main.o http_server.o http_client.o speak.o listen.o matrix_leds.o matrix_mics.o snowboy_wrapper.o
	$(CXX) $^ -o $@ $(CXXFLAGS) $(PATH_LIB) $(LIBS)

%.o: %.cpp %.h
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

%.o: %.cc  %.h
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

%.o: %.c  %.h
	$(CXX) -c $< $(PATH_INC) -o $@ $(CXXFLAGS)

.PHONY: install
install: $(EXE)
	mkdir -p ~/butler-client
	cp $< ~/butler-client/$(EXE)
	cp -r resources/ ~/butler-client/

.PHONY: clean
clean:
	rm -rf a.out *.o *.bak sarah sarah-record
