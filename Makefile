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

test: $(TARGET)
	@echo "🔍 Mehrere Tests werden ausgeführt..."
	@mkdir -p test_results

	# 🔹 Test 1: Normale Wortsuche
	@echo "📌 Test 1: Suche nach 'Dorf' in test1.txt" > test_results/test1_output.txt
	@./$(TARGET) "Dorf" test/test1.txt >> test_results/test1_output.txt && echo "✅ Test 1 erfolgreich!" || echo "❌ Test 1 fehlgeschlagen!"

	# 🔹 Test 2: Suche ohne Berücksichtigung der Groß- und Kleinschreibung (-i)
	@echo "📌 Test 2: Suche nach 'dorf' (case-insensitive)" > test_results/test2_output.txt
	@./$(TARGET) -i "dorf" test/test1.txt >> test_results/test2_output.txt && echo "✅ Test 2 erfolgreich!" || echo "❌ Test 2 fehlgeschlagen!"

	# 🔹 Test 3: Rekursive Suche in allen Dateien (-r)
	@echo "📌 Test 3: Rekursive Suche nach 'Buch'" > test_results/test3_output.txt
	@./$(TARGET) -r "Buch" test/ >> test_results/test3_output.txt && echo "✅ Test 3 erfolgreich!" || echo "❌ Test 3 fehlgeschlagen!"

	# 🔹 Test 4: Suche mit Zeilennummern (-n)
	@echo "📌 Test 4: Suche mit Zeilennummern nach 'gold'" > test_results/test4_output.txt
	@./$(TARGET) -n "gold" test/test2.txt >> test_results/test4_output.txt && echo "✅ Test 4 erfolgreich!" || echo "❌ Test 4 fehlgeschlagen!"

	# 🔹 Test 5: Wort nicht gefunden (sollte eine Fehlermeldung ausgeben)
	@echo "📌 Test 5: Suche nach 'XYZ123' (sollte nicht gefunden werden)" > test_results/test5_output.txt
	@./$(TARGET) "XYZ123" test/test1.txt >> test_results/test5_output.txt && echo "✅ Test 5 erfolgreich!" || echo "❌ Test 5 fehlgeschlagen!"

	# 🔹 Test 6: Wortzählung in allen Dateien (-c)
	@echo "📌 Test 6: Zählung des Wortes 'die' in allen Testdateien" > test_results/test6_output.txt
	@./$(TARGET) -c "die" test/*.txt >> test_results/test6_output.txt && echo "✅ Test 6 erfolgreich!" || echo "❌ Test 6 fehlgeschlagen!"

	@echo "✅ Alle Tests abgeschlossen! Ergebnisse befinden sich im Ordner 'test_results/'."

.PHONY: all clean test
