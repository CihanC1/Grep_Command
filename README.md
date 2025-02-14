# Grep-Befehl - Erweiterte Textsuche in Dateien

## Übersicht  
Der **Grep-Befehl** ist ein leistungsstarkes Befehlszeilentool zur effizienten Suche nach Textmustern in Dateien und Verzeichnissen. Es unterstützt verschiedene Filteroptionen, mehrfädige rekursive Suche und farblich hervorgehobene Ausgaben zur besseren Lesbarkeit.  

## Funktionen  
- **Groß-/Kleinschreibung ignorieren** (`-i`): Führt eine Suche ohne Beachtung der Groß-/Kleinschreibung durch.  
- **Negierte Suche** (`-v`): Zeigt Zeilen an, die NICHT mit dem Suchbegriff übereinstimmen.  
- **Treffer zählen** (`-c`): Zeigt die Gesamtzahl der Treffer an.  
- **Zeilennummer anzeigen** (`-n`): Gibt die Zeilennummer für gefundene Treffer aus.  
- **Rekursive Suche** (`-r`): Durchsucht Verzeichnisse rekursiv.  
- **Farbige Hervorhebung**: Übereinstimmende Ergebnisse werden farblich markiert.  
- **Unterstützung für STDIN**: Kann Eingaben aus einer Pipe (`|`) verarbeiten.  
- **Mehrfädige Verarbeitung**: Nutzt mehrere Threads für eine schnellere Suche.  

## Installation  
### Voraussetzungen  
Stellen Sie sicher, dass die folgenden Abhängigkeiten installiert sind:  
- `gcc` (C-Compiler)  
- `make` (Build-System)  
- `pthread` (Threading-Bibliothek)  

### Build-Anleitung  
Zum Kompilieren und Erstellen des Programms führen Sie aus:  
```sh  
make clean && make all 
```  
Dies generiert die ausführbare Datei `mygrep`.  

##  Verwendung  
### Grundlegende Syntax  
```sh  
./mygrep [OPTIONEN] "SUCHBEGRIFF" DATEI1 [DATEI2 ...]  
```  

### Beispielbefehle  
1. **Suche nach einem Wort in einer Datei:**  
   ```sh  
   ./mygrep "Fehler" logfile.txt  
   ```  
2. **Groß-/Kleinschreibung ignorieren:**  
   ```sh  
   ./mygrep -i "warnung" logfile.txt  
   ```  
3. **Zeilennummern anzeigen:**  
   ```sh  
   ./mygrep -n "fehlgeschlagen" logfile.txt  
   ```  
4. **Rekursiv in einem Verzeichnis suchen:**  
   ```sh  
   ./mygrep -r "main" src/  
   ```  
5. **Anzahl der Vorkommen eines Begriffs zählen:**  
   ```sh  
   ./mygrep -c "TODO" projekt/  
   ```  
6. **STDIN-Eingabe über eine Pipe durchsuchen:**  
   ```sh  
   cat file.txt | ./mygrep "suche"  
   ```  


## 📌 Automatisierte Tests (`make test`)
Das Projekt enthält eine Test-Suite, die mit `make test` ausgeführt werden kann.

### **Testausführung:**
1️⃣ **Wechseln Sie in das Projektverzeichnis:**
   ```sh
   cd /path/to/Grep_Command
   ```
2️⃣ **Führen Sie die Tests aus:**
   ```sh
   make test
   ```
3️⃣ **Erfolgreiche Tests zeigen eine ✅ Meldung, fehlgeschlagene Tests eine ❌ Meldung.**

### **Testresultate anzeigen:**
Alle Testausgaben werden im Verzeichnis `test_results/` gespeichert. Um Ergebnisse zu sehen:
```sh
cat test_results/test1_output.txt
cat test_results/test2_output.txt
cat test_results/test3_output.txt
```

### **Durchgeführte Tests:**
| **Test-Name** | **Beschreibung** | **Befehl** |
|-------------|-------------|-----------|
| **Test 1** | Suche nach "Dorf" in `test1.txt`. | `./mygrep "Dorf" test/test1.txt` |
| **Test 2** | Groß-/Kleinschreibung ignorieren. | `./mygrep -i "dorf" test/test1.txt` |
| **Test 3** | Rekursive Suche in allen Testdateien. | `./mygrep -r "Buch" test/` |
| **Test 4** | Zeilennummern anzeigen. | `./mygrep -n "gold" test/test2.txt` |
| **Test 5** | Suche nach nicht existierendem Wort. | `./mygrep "XYZ123" test/test1.txt` |
| **Test 6** | Zählung des Wortes "die" in allen Dateien. | `./mygrep -c "die" test/*.txt` |

✅ Falls ein Test fehlschlägt, überprüfen Sie die Testausgaben und die Logs.

## 📜Lizenz  
Dieses Projekt ist **Open-Source** und steht unter der **MIT-Lizenz** zur Verfügung.  

##  Mitwirken  
Sie können gerne beitragen, indem Sie **Fehlermeldungen**, **Feature-Anfragen** oder **Pull-Requests** einreichen.  

## 📧 Kontakt  
Für Fragen oder Anregungen kontaktieren Sie:  
- **Yusuf Oemer Karatas**: [yusufoemer.karatas@study.thws.de](mailto:yusufoemer.karatas@study.thws.de) - [GitHub](https://github.com/yusufoemerkaratas)  
- **Cihan Can**: [cihan.can@study.thws.de](mailto:cihan.can@study.thws.de) - [GitHub](https://github.com/CihanC1)  
- **Beyza Betul Ay**: [beyzabetuel.ay@study.thws.de](mailto:beyzabetuel.ay@study.thws.de) - [GitHub](https://github.com/Bbeyzay)  
- **Yusuferen Colak**: [yusuf.colak@study.thws.de](mailto:yusuf.colak@study.thws.de) - [GitHub](https://github.com/yusuferencolak) 
