#!/bin/bash

# Zu testendes Programm
EXEC="./mygrep"

# Testverzeichnis und Dateien vorbereiten
TEST_DIR="testverzeichnis"
TEST_FILE="${TEST_DIR}/test_datei.txt"
RECURSIVE_FILE="${TEST_DIR}/test_rekursiv.txt"

mkdir -p $TEST_DIR

echo "Dies ist eine Testdatei." > $TEST_FILE
echo "Dies ist ein GREP-Test." >> $TEST_FILE
echo "Eine weitere Zeile." >> $TEST_FILE
echo "Farbige GREP-Ausgabe." >> $TEST_FILE

echo "GREP Verzeichnis-Test" > $RECURSIVE_FILE

# Test 1: Einfache Suche
EXPECTED="Dies ist ein GREP-Test."
RESULT=$($EXEC "GREP" $TEST_FILE)
if echo "$RESULT" | grep -q "$EXPECTED"; then
    echo "[PASS] GREP wird in test_datei.txt gefunden"
else
    echo "[FAIL] GREP wird in test_datei.txt nicht gefunden"
    echo "Erwartet: '$EXPECTED'"
    echo "Erhalten: '$RESULT'"
fi

# Test 2: Groß-/Kleinschreibung ignorieren (-i)
EXPECTED="Dies ist ein GREP-Test.
Farbige GREP-Ausgabe."
RESULT=$($EXEC -i "grep" $TEST_FILE)
if echo "$RESULT" | grep -q "Dies ist ein GREP-Test."; then
    echo "[PASS] Suche ohne Berücksichtigung der Groß-/Kleinschreibung"
else
    echo "[FAIL] Suche ohne Berücksichtigung der Groß-/Kleinschreibung"
    echo "Erwartet: '$EXPECTED'"
    echo "Erhalten: '$RESULT'"
fi

# Test 3: Negierte Suche (-v)
EXPECTED="Dies ist eine Testdatei.
Eine weitere Zeile."
RESULT=$($EXEC -v "GREP" $TEST_FILE)
if echo "$RESULT" | grep -q "Dies ist eine Testdatei."; then
    echo "[PASS] Negierte Suche (Zeilen ohne Treffer)"
else
    echo "[FAIL] Negierte Suche (Zeilen ohne Treffer)"
    echo "Erwartet: '$EXPECTED'"
    echo "Erhalten: '$RESULT'"
fi

# Test 4: Zeilennummern anzeigen (-n)
EXPECTED="2:Dies ist ein GREP-Test.
4:Farbige GREP-Ausgabe."
RESULT=$($EXEC -n "GREP" $TEST_FILE)
if echo "$RESULT" | grep -q "2:Dies ist ein GREP-Test."; then
    echo "[PASS] Zeilennummern werden korrekt angezeigt"
else
    echo "[FAIL] Zeilennummern werden nicht korrekt angezeigt"
    echo "Erwartet: '$EXPECTED'"
    echo "Erhalten: '$RESULT'"
fi

# Test 5: STDIN mit Pipe testen
EXPECTED="GREP Test"
RESULT=$(echo "GREP Test" | $EXEC "GREP")
if echo "$RESULT" | grep -q "GREP Test"; then
    echo "[PASS] STDIN-Suche erfolgreich"
else
    echo "[FAIL] STDIN-Suche fehlgeschlagen"
    echo "Erwartet: '$EXPECTED'"
    echo "Erhalten: '$RESULT'"
fi

# Test 6: Rekursive Suche (-r)
EXPECTED="testverzeichnis/test_rekursiv.txt:GREP Verzeichnis-Test"
RESULT=$($EXEC -r "GREP" $TEST_DIR)
if echo "$RESULT" | grep -q "GREP Verzeichnis-Test"; then
    echo "[PASS] Rekursive Suche erfolgreich"
else
    echo "[FAIL] Rekursive Suche fehlgeschlagen"
    echo "Erwartet: '$EXPECTED'"
    echo "Erhalten: '$RESULT'"
fi

# Test 7: Ungültige Datei-Fehlermeldung
RESULT=$($EXEC "GREP" "nicht_existente_datei.txt" 2>&1)
if echo "$RESULT" | grep -q "No such file or directory"; then
    echo "[PASS] Fehlerbehandlung für ungültige Datei erfolgreich"
else
    echo "[FAIL] Fehlerbehandlung für ungültige Datei fehlgeschlagen"
    echo "Erwartet: 'No such file or directory'"
    echo "Erhalten: '$RESULT'"
fi

# Test 8: Leere Datei Test
touch leer.txt
RESULT=$($EXEC "GREP" leer.txt)
if [ -z "$RESULT" ]; then
    echo "[PASS] Leere Datei wird korrekt behandelt"
else
    echo "[FAIL] Leere Datei wird nicht korrekt behandelt"
    echo "Erwartet: ''"
    echo "Erhalten: '$RESULT'"
fi
rm leer.txt

echo "✔ Alle Tests abgeschlossen!"
