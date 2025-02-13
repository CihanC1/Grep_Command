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
make clean && make  
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



## 📜Lizenz  
Dieses Projekt ist **Open-Source** und steht unter der **MIT-Lizenz** zur Verfügung.  

##  Mitwirken  
Sie können gerne beitragen, indem Sie **Fehlermeldungen**, **Feature-Anfragen** oder **Pull-Requests** einreichen.  

## 📧 Kontakt  
Für Fragen oder Anregungen kontaktieren Sie:  
- **Yusuf Oemer Karatas**: [yusufoemer.karatas@study.thws.de](mailto:yusufoemer.karatas@study.thws.de) - [GitHub](https://github.com/yusufoemerkaratas)  
- **Cihan Can**: [cihan.can@study.thws.de](mailto:cihan.can@study.thws.de) - [GitHub](https://github.com/CihanC1)  
- **Beyza Betul Ay**: [beyzabetul.ay@study.thws.de](mailto:beyzabetul.ay@study.thws.de) - [GitHub](https://github.com/kapheria)  
- **Yusuferen Colak**: [yusuf.colak@study.thws.de](mailto:yusuf.colak@study.thws.de) - [GitHub](https://github.com/yusuferencolak) 
