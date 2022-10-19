![image](https://user-images.githubusercontent.com/3693209/196634372-ca180ffc-9073-49d0-87bd-1a6f43d3d826.png)

# RC Crawler Controller
Ermöglicht das Licht zu steuern für den RC Crawler. Die Steuerung erfolgt über das Einlesen der Signale aus dem RC Empfänger über mehrere Kanäle.
Aktuell werden drei zwei verwendet, die von der Fernsteuerung drei Zustände sendet. Zudem kommt als drittes, das einlesen des Signals, welches zum Lenken verwendet wird.

## Hardware
- Arduino Mini
- RGB LEDs, WS2812b
  - Vordere Stoßstangen Beleuchtung
  - Vordere Dach Beleuchtung
- PCA9685
- 12 LEDs
  - 2 weiße LEDs, vorderes Standlicht
  - 2 weiße LEDs, vorderes Fahrlicht
  - 2 rote LEDs, hinteres Fahrlicht
  - 4 orange LEDs, vordere und hintere Blinker
  - 2 weiße SMD LEDs, hinteres Nummernschild

### Lenken
- Mitte Gerade, LEDs für Blinker sind alle aus
- Links lenken, beide LEDs für Linke Blinker leuchten
- Rechts lenken, beide LEDs für rechts Blinker leuchten

### Schalter 1, drei Zustände
1. Alle LEDs aus
2. LEDs für  Standlicht und LEDs für das Nummernschild
3. LEDs für das Fahrlicht
   - Vorderes und hinteres Fahrlicht
   - Standlicht aus

### Schalter 2, drei Zustände
1. Alle RGBs aus
2. Alle Weiß leuchtend
3. Alle RGBs an
   - Lila
   - Stoßstangen Beleuchtung Iconisches Lauflicht wie bei Knight Rider
   - Dach Beleuchtung, animiertes Lauflicht von Innen nach außen.
