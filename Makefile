# Derleyici Seçimi
ifeq ($(OS),Windows_NT)
    CC = x86_64-w64-mingw32-gcc
    RM = del /f /q
    EXE = .exe
else
    CC = gcc
    RM = rm -f
    EXE =
endif

# Derleme Ayarları
CFLAGS = -std=c11 -Wall -Wextra -pthread -Iproject/modules

# Hedef ve Kaynak Dosyalar
TARGET = mygrep
SOURCES = project/src/main.c project/src/search.c project/src/globals.c project/src/args_parser.c
OBJECTS = $(SOURCES:.c=.o)

# Varsayılan Hedef
all: $(TARGET)$(EXE)

# Derleme Komutu
$(TARGET)$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET)$(EXE) $(OBJECTS)

# Object Dosyalarının Derlenmesi
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Temizleme Komutu
clean:
	-$(RM) project/src/*.o $(TARGET)$(EXE) 2>nul || true

# Test Komutu
test: $(TARGET)$(EXE)
	./$(TARGET)$(EXE) -c "deneme" test1.txt

.PHONY: all clean test
