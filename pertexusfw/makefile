ARDUINOROOT='/cygdrive/c/Program Files (x86)/Arduino'
GCCTOOLS=$(ARDUINOROOT)/hardware/tools/arm
INCLUDEDIRS="$(ARDUINOROOT)/hardware/teensy/avr/cores/teensy4" \
	"$(ARDUINOROOT)/hardware/teensy/avr/libraries/Wire" \
	"$(ARDUINOROOT)/hardware/teensy/avr/libraries/SD/src" \
	"$(ARDUINOROOT)/hardware/teensy/avr/libraries/SdFat/src" \
	"$(ARDUINOROOT)/hardware/teensy/avr/libraries/SPI"
INCLUDES=$(shell echo $(INCLUDEDIRS) | xargs -n 1 cygpath -m | \
	    xargs -I x echo -I\'x\')

ARMGCC=$(GCCTOOLS)/bin/arm-none-eabi-g++
CFLAGS_CPP=-O2 -g -Wall -ffunction-sections -fdata-sections -nostdlib \
	-MMD -std=gnu++14 -fno-exceptions -fpermissive -fno-rtti \
	-fno-threadsafe-statics -felide-constructors -Wno-error=narrowing \
	-mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 \
	-D__IMXRT1062__ -DTEENSYDUINO=156 -DARDUINO=10819 -DARDUINO_TEENSY41 \
	-DF_CPU=600000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH

%.o: %.ino
#	echo $(INCLUDEDIRS) | xargs -n 1 echo
	$(ARMGCC) -c $(CFLAGS_CPP) $(INCLUDES) \
	    -include Arduino.h \
	-o $@ -x c++ $(shell cygpath -m "$<")

%.i: %.ino
	$(ARMGCC) -E $(CFLAGS_CPP) $(INCLUDES) \
	    -include Arduino.h \
	-o $@ -x c++ $(shell cygpath -m "$<")
