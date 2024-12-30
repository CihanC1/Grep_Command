CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread -I../modules
TARGET = mygrep
SOURCES = main.c search.c globals.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: main.c ../modules/globals.h ../modules/search.h
	$(CC) $(CFLAGS) -c main.c

search.o: search.c ../modules/globals.h ../modules/search.h
	$(CC) $(CFLAGS) -c search.c

globals.o: globals.c ../modules/globals.h
	$(CC) $(CFLAGS) -c globals.c

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	./$(TARGET) --test

.PHONY: all clean test

