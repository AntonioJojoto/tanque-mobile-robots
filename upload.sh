#!/bin/sh


arduino-cli compile -b arduino:avr:uno -e tanque-mobile-robots.ino
cd build/arduino.avr.uno/
sudo avrdude -c usbtiny -p m328p -U flash:w:tanque-mobile-robots.ino.hex
