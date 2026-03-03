SRCS = $(wildcard *.cpp)
OBJECTS = $(SRCS:.c=.o)

cc = g++
#FLAGS = -W -Wall -I./CommUtil -I./include -std=c++17
FLAGS = -W -I../CommUtil -I../include -std=c++17
TARGET = pgc
DEBUG = pgcd

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(cc) $(FLAGS) -o $@ $^

d : $(OBJECTS)
	$(cc) $(FLAGS) -g -o $(DEBUG) $^

clean :
	rm -rf *.o $(TARGET) $(DEBUG)
