# Pertexus

## Introduction
Pertexus is a board design and firmware to allow control of
Pertex-type tape interface devices.  The board is designed around
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
date 080113012022
```

Sets the date to August 1, 2022, 1:01PM.


### density

### echo

### fsf

### help

### nop

### read

### reset

### rewind

### sd

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

