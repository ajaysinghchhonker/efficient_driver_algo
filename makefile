CFLAGS = -Wall
CVERSION = -std=gnu++11

SOURCES = main.cpp driver.cpp

all:	program

program: driver.o main.o
	g++ $(CVERSION) $(CFLAGS) -o main main.o driver.o
#g++ $(CVERSION) $(CFLAGS) -o main $(SOURCES)

driver.o:	driver.cpp driver.h
	g++ $(CVERSION) $(CFLAGS) -c driver.cpp

main.o: main.cpp
	g++ $(CVERSION) $(CFLAGS) -c main.cpp

clean:
	rm *.o

clean_all:
	rm *.o main
