#!/bin/bash

# Ana test dizini
TEST_DIR="test_directory"

# Alt dizinler
DIR1="$TEST_DIR/subdir1"
DIR2="$TEST_DIR/subdir2"

# Dizinleri oluştur
mkdir -p "$DIR1"
mkdir -p "$DIR2"

# Metin dosyaları ve içerikleri
echo -e "Dies ist eine Testdatei.\nWir testen die Suche.\nDas ist eine weitere Zeile." > "$DIR1/file1.txt"
echo -e "Hallo Welt!\nDies ist ein weiterer Test.\nSuchen Sie nach etwas?" > "$DIR1/file2.txt"
echo -e "Programmieren auf Deutsch.\nCoden macht Spaß!\nDiese Datei enthält Testdaten." > "$DIR2/file3.txt"
echo -e "Ein weiteres Beispiel.\nThreads und Prozesse.\nAlles funktioniert korrekt." > "$DIR2/file4.txt"

# Oluşturulan dosyaları listele
echo "📁 Test dizin yapısı oluşturuldu:"
tree "$TEST_DIR"

