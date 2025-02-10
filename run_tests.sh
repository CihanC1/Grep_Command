#!/bin/bash

# ANSI renk kodları
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Test dosyalarını oluştur
echo -e "This is a test file.\nAnother test line.\nNo match here." > test1.txt
echo -e "Test case should work.\nAnother example.\nThis line has no match." > test2.txt

# Test fonksiyonu
run_test() {
    local description=$1
    local command=$2
    local expected_output_file=$3

    echo -e "=============================="
    echo -e "🔹 Running test: ${description}"
    echo -e "📌 Executing: ${command}"

    # ANSI renk kodlarını kaldır ve çıktıyı dosyaya yaz
    eval "$command" | sed 's/\x1b\[[0-9;]*m//g' | tr -d '\r' > actual_output.txt  

    if diff -u "$expected_output_file" actual_output.txt > /dev/null; then
        echo -e "${GREEN}✔ PASSED${NC}"
    else
        echo -e "${RED}✘ FAILED${NC}"
        echo "📌 Expected Output:"
        cat "$expected_output_file"
        echo "📌 Actual Output:"
        cat actual_output.txt
    fi
}

# Beklenen çıktıları dosyalara yaz
echo -e "test1.txt: This is a test file.\ntest1.txt: Another test line." > expected1.txt
echo -e "test2.txt: Test case should work." > expected2.txt
echo -e "test1.txt:1: This is a test file.\ntest1.txt:2: Another test line." > expected3.txt
echo -e "" > expected4.txt  # Case-sensitive başarısız test için boş dosya
echo -e "test2.txt: Test case should work." > expected5.txt  # Case-insensitive başarılı test

# Testleri çalıştır
run_test "Basic Search (test1.txt)" "./mygrep test test1.txt" "expected1.txt"
run_test "Basic Search (test2.txt) (Case-Sensitive - should fail)" "./mygrep test test2.txt" "expected4.txt"
run_test "Basic Search (test2.txt) (Case-Insensitive - should pass)" "./mygrep -i test test2.txt" "expected5.txt"
run_test "Show Line Numbers" "./mygrep -n test test1.txt" "expected3.txt"

# Geçici test dosyalarını temizle
rm -f test1.txt test2.txt actual_output.txt expected1.txt expected2.txt expected3.txt expected4.txt expected5.txt

echo -e "\n✅✅✅ ALL TESTS COMPLETED ✅✅✅"

