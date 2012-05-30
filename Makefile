BOARD_TAG			= uno
ARDUINO_PORT		= /dev/ttyACM0
ARDUINO_DIR			= /home/gabriel/bin/arduino
ARDMK_DIR     		= ../Arduino-Makefile
AVR_TOOLS_DIR	 	= /usr
ARDUINO_SKETCHBOOK	= /home/gabriel/dev/arduino

ARDUINO_LIBS 		= Si4735 SPI Wire Wire/utility

include ../Arduino-Makefile/arduino-mk/Arduino.mk
#~ include ../ardmk/Arduino.mk
