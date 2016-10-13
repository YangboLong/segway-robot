EXEC    = bin/segwayRobot
OBJS    = bin/segwayRobot.o bin/control.o
OBJS   += bin/extApi.o bin/extApiPlatform.o
LIBS    = -lpthread

CFLAGS  = -Wall -O0 -ggdb
CFLAGS += -I../libraries/remoteApi -I../libraries/include
CFLAGS += -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux
else
	CFLAGS += -D__APPLE__
endif

all:
	@rm -f bin/*
	@mkdir -p bin
	g++ $(CFLAGS) -o bin/segwayRobot.o -c segwayRobot.cpp
	g++ $(CFLAGS) -o bin/control.o -c control.cpp
	gcc $(CFLAGS) -o bin/extApi.o -c ../libraries/remoteApi/extApi.c
	gcc $(CFLAGS) -o bin/extApiPlatform.o -c ../libraries/remoteApi/extApiPlatform.c
	g++ -o $(EXEC) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	@rm -f bin/*
