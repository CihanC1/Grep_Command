CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread -Iproject/modules
TARGET = mygrep
SOURCES = project/src/main.c project/src/search.c project/src/globals.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

project/src/main.o: project/src/main.c project/modules/globals.h project/modules/search.h
	$(CC) $(CFLAGS) -c project/src/main.c -o project/src/main.o

project/src/search.o: project/src/search.c project/modules/globals.h project/modules/search.h
	$(CC) $(CFLAGS) -c project/src/search.c -o project/src/search.o

project/src/globals.o: project/src/globals.c project/modules/globals.h
	$(CC) $(CFLAGS) -c project/src/globals.c -o project/src/globals.o

clean:
	rm -f project/src/*.o mygrep

test: $(TARGET)
	./$(TARGET) "deneme" test1.txt

.PHONY: all clean test
