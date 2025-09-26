@echo off
avrdude -c wiring -p atmega2560 -P COM12 -b 115200 -D -U flash:w:"%1\%1\Release\%1.hex":i