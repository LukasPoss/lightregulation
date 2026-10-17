# lightregulation
# KIM – Lichtregulierungseinheit

## Projektbeschreibung

Dieses Repository enthält die Firmware und die technischen Unterlagen zur Lichtregulierungseinheit der KIM-Insektenmonitoringstation am IparLab des Umwelt-Campus Birkenfeld unter der Leitung von Prof. Dr. Stephan Didas.

Ziel der Einheit ist es, eine möglichst konstante Beleuchtung für die Beobachtung und Aufzeichnung von Insekten bereitzustellen. Die Beleuchtung wird abhängig von der gemessenen Umgebungshelligkeit geregelt. Gleichzeitig berücksichtigt die Steuerung die Anforderungen der biologischen Beobachtung: Bei Dunkelheit soll keine künstliche Beleuchtung eingesetzt werden, die nachtaktive Insekten zusätzlich anziehen könnte.

Die Lichtregulierungseinheit wurde von mir eigenständig konzipiert, geplant und umgesetzt. Dazu gehören die Entwicklung der Elektronik, die Firmware, die Konstruktion der Gehäuse und Halterungen sowie die Integration der Beleuchtung in die Station.

Insgesamt wurden vier Einheiten aufgebaut und eine wurde in Betrieb genommen.

## Funktionsweise

Die Umgebungshelligkeit wird mit einem VEML7700-Lichtsensor gemessen. Ein ESP32-C3 Super Mini verarbeitet die Messwerte und steuert auf dieser Grundlage die Helligkeit der NeoPixel-Beleuchtung.

Um kurzfristige Schwankungen der Messwerte auszugleichen, werden mehrere Messungen zu einem Mittelwert zusammengefasst. Die Helligkeit wird anschließend schrittweise angepasst, damit keine abrupten Helligkeitssprünge entstehen.

### Regelungsverhalten

| Gemessene Umgebungshelligkeit | Verhalten |
|---|---|
| Bis einschließlich 30 Lux | Nach drei Sekunden unterhalb des Grenzwerts wird die Beleuchtung schrittweise heruntergeregelt. |
| Über 30 bis 600 Lux | Die Sollhelligkeit wird abhängig vom Messwert linear zwischen 80 % und 25 % des maximalen NeoPixel-Helligkeitswerts berechnet. |
| Über 600 Lux | Der Messwert wird für die Berechnung auf 600 Lux begrenzt. Die Beleuchtung wird somit auf ungefähr 25 % geregelt und nicht vollständig abgeschaltet. |

Die Firmware verwendet folgende Parameter:

- **Messintervall:** 200 ms
- **Regelungsintervall:** 2 Sekunden
- **Messwertglättung:** Mittelwert aus bis zu 25 Messungen
- **Änderung pro Regelungsschritt:** maximal 8 Helligkeitseinheiten
- **Minimaler Sollwert im Regelbereich:** 25 % von 255
- **Maximaler Sollwert im Regelbereich:** 80 % von 255

Die schrittweise Anpassung sorgt dafür, dass die Beleuchtung nicht bei jeder Änderung des Messwerts unmittelbar auf eine neue Helligkeit springt.

### Besonderheit: Nachtruhe

Bei einer Umgebungshelligkeit von höchstens 30 Lux wird nach Ablauf der Wartezeit die Beleuchtung heruntergeregelt. Dadurch soll verhindert werden, dass die Station nachts durch künstliches Licht zusätzliche Insekten anzieht.

Die Lichtsteuerung orientiert sich somit nicht ausschließlich an der technischen Anforderung einer ausreichenden Beleuchtung, sondern berücksichtigt auch die wissenschaftlichen Anforderungen an möglichst unbeeinflusste Beobachtungen.

## Hardware

### Verwendete Komponenten

| Komponente | Aufgabe |
|---|---|
| ESP32-C3 Super Mini | Verarbeitung der Messwerte und Steuerung der Beleuchtung |
| VEML7700 | Messung der Umgebungshelligkeit |
| 18 NeoPixel-LEDs | Indirekte Beleuchtung der Station |
| 470-Ω-Widerstand | Widerstand in der Datenleitung der NeoPixel |
| 100-µF-Elektrolytkondensator | Unterstützung der Stabilisierung der Versorgungsspannung |
| 5-V-Netzteil mit 3 A | Stromversorgung der Einheit |
| Kühlkörper | Zusätzliche Kühlung des ESP32-C3 |

### Elektronik und Aufbau

Die Platine wurde anhand der benötigten Funktionen eigenständig konzipiert und aufgebaut. Bei der Planung wurden die Anschlüsse für den Controller, den Lichtsensor und die LED-Beleuchtung sowie die benötigten Komponenten zur Versorgung und Signalübertragung berücksichtigt.

Der Aufbau umfasst:

- einen Anschluss für die Stromversorgung,
- einen vierpoligen Anschluss für den VEML7700,
- einen dreipoligen Anschluss für die NeoPixel,
- einen 470-Ω-Widerstand in der LED-Datenleitung,
- einen 100-µF-Elektrolytkondensator zur Unterstützung der Spannungsstabilität.

Die Versorgung erfolgt über ein handelsübliches 5-V-Netzteil mit einer Nennleistung von 3 A.

### Positionierung des Lichtsensors

Der VEML7700 ist nicht direkt auf der Steuerplatine montiert. Die Platine befindet sich im Schaltschrank der Station, während der Sensor im Gehäuse der Station am Mast sitzt. Beide sind über ein ungefähr 2,5 m langes Kabel miteinander verbunden.

Das Sensorgehäuse ist zur Außenseite hin halbtransparent ausgeführt. Drei Seiten sind lichtundurchlässig gestaltet, damit möglichst kein Licht der eigenen Beleuchtung auf den Sensor fällt.

Die Beleuchtung ist indirekt ausgeführt und strahlt nicht unmittelbar in die Kameraoptik. Durch die räumliche Trennung von Sensor und Beleuchtung wird außerdem vermieden, dass die Regelung unmittelbar auf das von den eigenen LEDs erzeugte Licht reagiert. Der Sensor erfasst damit das von außen einfallende Umgebungslicht.

## Mechanische Konstruktion

Neben der Elektronik wurden die benötigten mechanischen Bauteile konstruiert und für den 3D-Druck vorbereitet. Die entsprechenden Dateien liegen im Ordner `components/` im STEP-Format vor.

Dazu gehören:

- Halterung für die Platine,
- Gehäuse für die Platine,
- Gehäuse für den VEML7700,
- Bauteile für die LED-Beleuchtung.

Der Beleuchtungsrahmen erfüllt dabei eine zusätzliche Funktion: Er dient zugleich als Kabelführung innerhalb der Station. Die Leitungen der drei 4K-Kameras sowie die Kabel der NeoPixel-Beleuchtung und des Lichtsensors werden im Beleuchtungsgehäuse geführt.

Dadurch sollen Vibrationen und frei bewegliche Kabel möglichst wenig auf den Kamerabereich übertragen werden, damit die Aufnahmen nicht durch unnötige Bewegungen oder Störungen beeinträchtigt werden.

Das STEP-Format wurde gewählt, um die mechanischen Bauteile einheitlich mit weiteren Konstruktionsteilen der Station austauschen und weiterverarbeiten zu können.

## Software

Die Firmware wurde für den ESP32-C3 Super Mini mit der Arduino IDE entwickelt.

### Verwendete Bibliotheken

- [Adafruit VEML7700 Library](https://github.com/adafruit/Adafruit_VEML7700) – Kommunikation mit dem Lichtsensor
- [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel) – Ansteuerung der NeoPixel-Beleuchtung

Die I²C-Kommunikation mit dem Sensor und die Steuerung der LED-Datenleitung erfolgen über die im Sketch definierten GPIO-Pins.

| Funktion | ESP32-C3 GPIO |
|---|---:|
| I²C SDA | GPIO 8 |
| I²C SCL | GPIO 9 |
| NeoPixel-Datenleitung | GPIO 3 |

Die Firmware verwendet die RGB-Farbkonfiguration mit einer Übertragungsfrequenz von 800 kHz. Alle 18 LEDs werden mit identischen RGB-Werten angesteuert, sodass eine weiße Beleuchtung entsteht.

### Firmware aufspielen

1. Die [Arduino IDE 2.x](https://www.arduino.cc/en/software) installieren.
2. Die benötigten ESP32-Boarddefinitionen für die Arduino IDE einrichten.
3. Über den Bibliotheksverwalter die Bibliotheken `Adafruit VEML7700` und `Adafruit NeoPixel` installieren.
4. Den Sketch aus dem Ordner `sketch/` öffnen.
5. Das verwendete Board **`Nologo ESP32C3 Super Mini`** auswählen.
6. Den passenden COM-Port auswählen und den Sketch auf den ESP32-C3 hochladen.

Die Auswahl des richtigen Boardprofils ist wichtig, damit die Pinbelegung und die Upload-Konfiguration zum verwendeten ESP32-C3 Super Mini passen.

Nach dem Start initialisiert die Firmware den Sensor und die LED-Steuerung. Über die serielle Schnittstelle können Messwerte und die aktuelle Helligkeit mit einer Baudrate von 115200 verfolgt werden.

Wird der VEML7700 bei der Initialisierung nicht erkannt, gibt die Firmware eine Fehlermeldung aus und bleibt anschließend in einer Warteschleife stehen.

## Repository-Struktur

```text
.
├── components/
│   └── 3D-Konstruktionsteile im STEP-Format
├── circuit_diagrams/
│   ├── Schaltplan als PNG
│   ├── Foto der Platine
│   └── info.txt
├── photos/
│   └── Fotos der aufgebauten Lichtregulierungseinheiten
├── sketch/
│   └── Firmware für den ESP32-C3
├── LICENSE
└── README.md
```

## Projektstatus

Die Lichtregulierungseinheit wurde aufgebaut und in die KIM-Insektenmonitoringstation integriert. Insgesamt wurden vier Einheiten realisiert.

Das Repository dokumentiert die von mir entwickelte technische Lösung einschließlich Firmware, Schaltplan, Fotos und mechanischer Konstruktionsteile.

## Lizenz

Die Lizenzbedingungen sind der Datei `LICENSE` in diesem Repository zu entnehmen.
