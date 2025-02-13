ifeq ($(OS),Windows_NT)
    CC = x86_64-w64-mingw32-gcc
    RM = del /f /q
    EXE = .exe
else
    CC = gcc
    RM = rm -f
    EXE =
endif

CFLAGS = -std=c11 -Wall -Wextra -pthread -Iproject/modules
LDFLAGS = -lc  
OBJ_DIR = project/src

TARGET = mygrep
SOURCES = $(OBJ_DIR)/main.c $(OBJ_DIR)/search.c $(OBJ_DIR)/globals.c $(OBJ_DIR)/args_parser.c $(OBJ_DIR)/thread.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)$(EXE)

$(TARGET)$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET)$(EXE) $(OBJECTS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(OBJ_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) $(OBJ_DIR)/*.o $(TARGET)$(EXE) 2>/dev/null || true

test: $(TARGET)$(EXE)
	./$(TARGET)$(EXE) -c "Test" test1.txt

.PHONY: all clean test
