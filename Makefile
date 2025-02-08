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
LDFLAGS = -lc  # Standart C kütüphanesi bağlantısı (özellikle MinGW için gerekli)
OBJ_DIR = project/src

# Hedef ve Kaynak Dosyalar
TARGET = mygrep
SOURCES = $(OBJ_DIR)/main.c $(OBJ_DIR)/search.c $(OBJ_DIR)/globals.c $(OBJ_DIR)/args_parser.c
OBJECTS = $(SOURCES:.c=.o)

# Varsayılan Hedef
all: $(TARGET)$(EXE)

# Derleme Komutu
$(TARGET)$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET)$(EXE) $(OBJECTS) $(LDFLAGS)

# Object Dosyalarının Derlenmesi
$(OBJ_DIR)/%.o: $(OBJ_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Temizleme Komutu
clean:
	-$(RM) $(OBJ_DIR)/*.o $(TARGET)$(EXE) 2>/dev/null || true

# Test Komutu
test: $(TARGET)$(EXE)
	./$(TARGET)$(EXE) -c "deneme" test1.txt

.PHONY: all clean test
