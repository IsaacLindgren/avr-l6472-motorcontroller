#!/bin/bash

echo "Programming AVR: "
avrdude -c usbtiny -p ATmega48 -U flash:w:MotorController.hex
