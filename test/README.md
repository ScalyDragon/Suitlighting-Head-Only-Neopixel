# Tests

Die Tests sind in zwei Ebenen getrennt:

- `test_native_core`: schnelle, hardwareunabhängige Regressionstests. Arduino-I/O
  wird durch kleine Test-Doubles ersetzt. Diese Tests prüfen Farbmathematik,
  Persistenz-Defaults, LED-Puffer und -Fades, Animator, Button-Entprellung,
  Lüfter-PWM/RPM sowie den analogen RGB-Treiber.
- `test_embedded_hardware`: Integrations- und Smoke-Tests auf einem echten ESP32.
  Sie greifen auf Touch, NeoPixel, EEPROM, SPIFFS, WLAN und Webserver zu und
  benötigen deshalb die passende Hardware und ein hochgeladenes Dateisystem.

## Native Tests ausführen

```sh
pio test -e native
```

Dieser Lauf ist für lokale Entwicklung und CI vorgesehen und benötigt keinen
angeschlossenen Mikrocontroller.

## Hardwaretests ausführen

Für das NodeMCU-32S:

```sh
pio run -e nodemcu-32s -t uploadfs
pio test -e nodemcu-32s
```

Für den Wemos D1 Mini ESP32:

```sh
pio run -e wemos_d1_mini32 -t uploadfs
pio test -e wemos_d1_mini32
```

Die im Hardwaretest definierten Pins müssen vor dem Lauf zur Verdrahtung passen.
Touch-Tests sind absichtlich nicht Teil des Native-Laufs, weil ihr Ergebnis von
realen Messwerten und der Kalibrierung abhängt.

## Vor einer Umstrukturierung

Mindestens diese Prüfungen ausführen:

```sh
pio test -e native
pio run -e nodemcu-32s
pio run -e wemos_d1_mini32
```

Wenn Hardware verfügbar ist, anschließend zusätzlich den entsprechenden
Hardwaretest ausführen. Ein erfolgreicher Build ersetzt keine Messung am Gerät.
