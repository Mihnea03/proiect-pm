# Sistem de Securitate cu Computer Vision

Munteanu Mihnea-Stefan (332CB)

## Descriere

Acest proiect este un sistem de securitate low-cost bazat pe ESP32-CAM. Sistemul detecteaza miscarea folosind un senzor PIR, transmite imagine live prin WiFi si afiseaza informatii pe un ecran LCD. In plus, emite un semnal sonor cand este detectata o fata.

Scopul proiectului este oferirea unei solutii simple si eficiente pentru supravegherea locuintelor sau a spatiilor mici.

## Componente hardware

- ESP32-CAM
- Placa ESP32-CAM MB (programator USB)
- Senzor PIR HC-SR501
- Modul buzzer activ
- LCD 1602 I2C
- Breadboard si fire jumper

Schema electrica si diagramele sunt in folderul `hardware/`.

## Functionalitati

- Detectie de miscare cu senzor PIR
- Transmisie video live prin WiFi
- Afisare IP si mesaje pe LCD
- Alarma sonora cand este detectata o fata
- Consum energetic redus

## Biblioteci utilizate

- esp_camera
- WiFi.h
- Wire.h
- LiquidCrystal_I2C.h
- ESPAsyncWebServer.h

## Instalare

1. Cloneaza repository-ul:

   ```bash
   git clone https://github.com/Mihnea03/proiect-pm.git
   ```

2. Deschide proiectul in VSCode cu PlatformIO

3. Conecteaza ESP32-CAM MB la PC si incarca proiectul

4. Acceseaza IP-ul afisat pe LCD in browser

## Rezultate

- Sistemul a fost testat complet pe breadboard
- Detectia de miscare si fata functioneaza
- Fluxul video este transmis cu succes prin retea
- LCD-ul si buzzer-ul reactioneaza corect

## Resurse

Software:

- https://github.com/grimmpp/esp32-cam-example

Hardware:

- https://www.handsontec.com/dataspecs/module/ESP32-CAM.pdf
- Magazine online: emag.ro, optimusdigital.ro

## Demo

https://youtu.be/PCld8pRs5Gw?si=RD_WUOPADhI0grXb
