OBJS	= main.o gaomon-s620.o
SOURCE	= main.cpp gaomon-s620.cpp
HEADER	= gaomon-s620.hpp
OUT	= driver
CC	= clang++
FLAGS	= -g -c -Wall
LFLAGS	= -lusb-1.0 -lrt

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

gaomon-s620.o: gaomon-s620.cpp
	$(CC) $(FLAGS) gaomon-s620.cpp


clean:
	rm -f $(OBJS)

clean-all:
	rm -f $(OBJS) $(OUT)
