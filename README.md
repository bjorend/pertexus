# Pertexus

## Introduction
Pertexus is a board design and firmware to allow control of
Pertec-type tape interface devices.  The board is designed around
the Teensy 4.1 board (https://www.pjrc.com/store/teensy41.html)

## Hardware Design

## Firmware

The firmware is built and programmed with the Arduino 1.8.19 GUI with the
Teensyduino 1.56 add-in (https://www.pjrc.com/teensy/td_download.html).


## Usage

The firmware presents a USB "serial" device to which a user can connect.
It implements a simple command line interface.

## Regular Commands

### date - Get or set the current date and time

#### Usage

date [MMDDhhmm[[CC]YY][.ss]]

#### Examples

To set the date:

```
PERTEXUS> date 112921532022
Tue Nov 29 21:53:08 2022
```

Sets the date to November 29, 2022, 9:53PM.


### density

### echo

### fsf

### help

### nop

### read

### reset

### rewind

### sd - Commands relating to SD storage

#### Usage
```
sd init|dir|read|rm
```

The `sd` command can:
* initialize communication with an SD card.  Note: it does not "initialize"
the SD card as in "format" it; the SD card must be already formatted in FAT32
format.
* list the contents of the SD card
* perform a test read of a tape file from the SD card (to measure performance)
* remove a single file from the SD card


### stats

### unload

### write

### erase

## Development/Debugging Commands

### cmd

### dump

### gpio

### peek

### play

### poke

### tapetest

### testread

### testwrite

