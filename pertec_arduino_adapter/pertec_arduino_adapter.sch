EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 1 1
Title "bdb004: Pertec/Arduino Mega 2560 R3 Adapter Board"
Date ""
Rev "Rev. A"
Comp "Bjoren Davis"
Comment1 ""
Comment2 "Design started April 6, 2022."
Comment3 "Arduino Mega 2560 R3 board to a Pertec magtape interface."
Comment4 "This board is an adapter to allow easy connection of an"
$EndDescr
$Comp
L Connector_Generic:Conn_02x25_Odd_Even J1
U 1 1 624E1B59
P 9550 2550
F 0 "J1" H 9600 3967 50  0000 C CNN
F 1 "Pertec J1" H 9416 3876 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x25_P2.54mm_Vertical" H 9550 2550 50  0001 C CNN
F 3 "~" H 9550 2550 50  0001 C CNN
	1    9550 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 1350 9250 1350
Wire Wire Line
	9250 1350 9250 1450
Wire Wire Line
	9250 1450 9350 1450
Wire Wire Line
	9250 1550 9250 1450
Connection ~ 9250 1450
Wire Wire Line
	9350 1550 9250 1550
Wire Wire Line
	9350 1650 9250 1650
Connection ~ 9250 1550
Wire Wire Line
	9350 1750 9250 1750
Wire Wire Line
	9250 1750 9250 1650
Connection ~ 9250 1650
Wire Wire Line
	9350 1850 9250 1850
Wire Wire Line
	9250 1850 9250 1750
Connection ~ 9250 1750
$Comp
L power:GNDREF #PWR0101
U 1 1 624E4CF2
P 8975 1900
F 0 "#PWR0101" H 8975 1650 50  0001 C CNN
F 1 "GNDREF" H 8975 1750 50  0000 C CNN
F 2 "" H 8975 1900 50  0001 C CNN
F 3 "" H 8975 1900 50  0001 C CNN
	1    8975 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1850 8975 1850
Wire Wire Line
	8975 1850 8975 1900
NoConn ~ 9350 1950
NoConn ~ 9350 2050
Wire Wire Line
	9350 2150 9250 2150
Wire Wire Line
	9250 2150 9250 2250
Wire Wire Line
	9250 2250 9350 2250
Wire Wire Line
	9350 2350 9250 2350
Wire Wire Line
	9250 2350 9250 2250
Connection ~ 9250 2250
Wire Wire Line
	9350 2450 9250 2450
Wire Wire Line
	9250 2450 9250 2350
Connection ~ 9250 2350
Wire Wire Line
	9350 2550 9250 2550
Wire Wire Line
	9250 2550 9250 2450
Connection ~ 9250 2450
Wire Wire Line
	9350 2650 9250 2650
Wire Wire Line
	9250 2650 9250 2550
Connection ~ 9250 2550
Wire Wire Line
	9350 2750 9250 2750
Wire Wire Line
	9250 2750 9250 2650
Connection ~ 9250 2650
Wire Wire Line
	9350 2850 9250 2850
Connection ~ 9250 2750
Wire Wire Line
	9350 2950 9250 2950
Wire Wire Line
	9250 2750 9250 2850
Connection ~ 9250 2850
Wire Wire Line
	9250 2850 9250 2950
Connection ~ 9250 1850
Wire Wire Line
	9250 1550 9250 1650
$Comp
L power:GNDREF #PWR0102
U 1 1 624EA927
P 8975 3000
F 0 "#PWR0102" H 8975 2750 50  0001 C CNN
F 1 "GNDREF" H 8975 2850 50  0000 C CNN
F 2 "" H 8975 3000 50  0001 C CNN
F 3 "" H 8975 3000 50  0001 C CNN
	1    8975 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 2950 8975 2950
Wire Wire Line
	8975 2950 8975 3000
Wire Wire Line
	9350 3150 9250 3150
Wire Wire Line
	9250 3150 9250 3250
Wire Wire Line
	9250 3250 9350 3250
Wire Wire Line
	9250 3250 9250 3350
Wire Wire Line
	9250 3350 9350 3350
Connection ~ 9250 3250
$Comp
L power:GNDREF #PWR0103
U 1 1 624EC769
P 8975 3400
F 0 "#PWR0103" H 8975 3150 50  0001 C CNN
F 1 "GNDREF" H 8975 3250 50  0000 C CNN
F 2 "" H 8975 3400 50  0001 C CNN
F 3 "" H 8975 3400 50  0001 C CNN
	1    8975 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 3350 8975 3350
Wire Wire Line
	8975 3350 8975 3400
NoConn ~ 9350 3050
Wire Wire Line
	9350 3550 9250 3550
Wire Wire Line
	9250 3550 9250 3650
Wire Wire Line
	9250 3650 9350 3650
Wire Wire Line
	9250 3650 9250 3750
Wire Wire Line
	9250 3750 9350 3750
Connection ~ 9250 3650
$Comp
L power:GNDREF #PWR0104
U 1 1 624EDD5A
P 8975 3800
F 0 "#PWR0104" H 8975 3550 50  0001 C CNN
F 1 "GNDREF" H 8975 3650 50  0000 C CNN
F 2 "" H 8975 3800 50  0001 C CNN
F 3 "" H 8975 3800 50  0001 C CNN
	1    8975 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 3750 8975 3750
Wire Wire Line
	8975 3750 8975 3800
NoConn ~ 9350 3450
Wire Wire Line
	11850 2900 11850 3000
Wire Wire Line
	11850 3100 11850 3000
Connection ~ 11850 3000
Wire Wire Line
	11850 3200 11850 3100
Connection ~ 11850 3100
Wire Wire Line
	11850 3300 11850 3200
Connection ~ 11850 3200
Wire Wire Line
	11850 3400 11850 3300
Connection ~ 11850 3300
Wire Wire Line
	11850 3500 11850 3400
Connection ~ 11850 3400
Connection ~ 11850 3500
Wire Wire Line
	11850 3500 11850 3600
Connection ~ 11850 3600
Wire Wire Line
	11850 3600 11850 3700
$Comp
L power:GNDREF #PWR0105
U 1 1 624F1453
P 11575 3750
F 0 "#PWR0105" H 11575 3500 50  0001 C CNN
F 1 "GNDREF" H 11575 3600 50  0000 C CNN
F 2 "" H 11575 3750 50  0001 C CNN
F 3 "" H 11575 3750 50  0001 C CNN
	1    11575 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	11850 3700 11575 3700
Wire Wire Line
	11575 3700 11575 3750
NoConn ~ 11950 3800
Wire Wire Line
	11850 2900 11850 2800
Connection ~ 11850 2900
Wire Wire Line
	11850 2400 11850 2500
Wire Wire Line
	11850 2600 11850 2500
Connection ~ 11850 2500
Wire Wire Line
	11850 2700 11850 2600
Connection ~ 11850 2600
Wire Wire Line
	11850 2800 11850 2700
Connection ~ 11850 2700
Wire Wire Line
	11850 2400 11850 2300
Connection ~ 11850 2400
Wire Wire Line
	11850 2200 11850 2100
Wire Wire Line
	11850 2300 11850 2200
Connection ~ 11850 2200
Connection ~ 11850 2100
Wire Wire Line
	11950 3700 11850 3700
Wire Wire Line
	11950 3600 11850 3600
Wire Wire Line
	11950 3500 11850 3500
Wire Wire Line
	11950 3400 11850 3400
Wire Wire Line
	11950 3300 11850 3300
Wire Wire Line
	11950 3200 11850 3200
Wire Wire Line
	11950 3100 11850 3100
Wire Wire Line
	11850 3000 11950 3000
Wire Wire Line
	11950 2900 11850 2900
Wire Wire Line
	11850 2800 11950 2800
Wire Wire Line
	11950 2700 11850 2700
Wire Wire Line
	11950 2600 11850 2600
Wire Wire Line
	11850 2500 11950 2500
Wire Wire Line
	11950 2400 11850 2400
Wire Wire Line
	11850 2300 11950 2300
Wire Wire Line
	11950 2200 11850 2200
Wire Wire Line
	11950 2100 11850 2100
Connection ~ 11850 1900
Wire Wire Line
	11850 1800 11950 1800
Wire Wire Line
	11850 1900 11850 1800
Connection ~ 11850 2000
Wire Wire Line
	11850 2100 11850 2000
Wire Wire Line
	11850 2000 11950 2000
Wire Wire Line
	11850 1900 11850 2000
Wire Wire Line
	11950 1900 11850 1900
$Comp
L Connector_Generic:Conn_02x25_Odd_Even J2
U 1 1 624DF2C8
P 12150 2600
F 0 "J2" H 12200 4017 50  0000 C CNN
F 1 "Pertec J2" H 12016 3926 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x25_P2.54mm_Vertical" H 12150 2600 50  0001 C CNN
F 3 "~" H 12150 2600 50  0001 C CNN
	1    12150 2600
	1    0    0    -1  
$EndComp
Connection ~ 11850 1700
Wire Wire Line
	11850 1800 11850 1700
Wire Wire Line
	11850 1700 11950 1700
Wire Wire Line
	11850 1600 11850 1700
Wire Wire Line
	11950 1600 11850 1600
Connection ~ 11850 1800
Connection ~ 11850 2300
Connection ~ 11850 2800
Wire Wire Line
	12450 1400 12625 1400
Text GLabel 12625 1400 2    50   Output ~ 0
IR0
Wire Wire Line
	12450 1500 12625 1500
Text GLabel 12625 1500 2    50   Output ~ 0
ILDP
Wire Wire Line
	12450 1600 12625 1600
Text GLabel 12625 1600 2    50   Output ~ 0
IR4
Wire Wire Line
	12450 1700 12625 1700
Text GLabel 12625 1700 2    50   Output ~ 0
IR7
Wire Wire Line
	12450 1800 12625 1800
Text GLabel 12625 1800 2    50   Output ~ 0
IR6
Wire Wire Line
	12450 1900 12625 1900
Text GLabel 12625 1900 2    50   Output ~ 0
IHER
Wire Wire Line
	12450 2000 12625 2000
Text GLabel 12625 2000 2    50   Output ~ 0
IFMK
Wire Wire Line
	12450 2100 12625 2100
Text GLabel 12625 2100 2    50   Output ~ 0
IDENT
Wire Wire Line
	12450 2200 12625 2200
Text GLabel 12625 2200 2    50   Input ~ 0
IFEN
Wire Wire Line
	12450 2300 12625 2300
Text GLabel 12625 2300 2    50   Output ~ 0
IR5
Wire Wire Line
	12450 2400 12625 2400
Text GLabel 12625 2400 2    50   Output ~ 0
IEOT
Wire Wire Line
	12450 2500 12625 2500
Text GLabel 12625 2500 2    50   Input ~ 0
IOFL
Wire Wire Line
	12450 2600 12625 2600
Text GLabel 12625 2600 2    50   Output ~ 0
INRZ
Wire Wire Line
	12450 2700 12625 2700
Text GLabel 12625 2700 2    50   Output ~ 0
IRDY
Wire Wire Line
	12450 2800 12625 2800
Text GLabel 12625 2800 2    50   Output ~ 0
IRWD
Wire Wire Line
	12450 2900 12625 2900
Text GLabel 12625 2900 2    50   Output ~ 0
IFPT
Wire Wire Line
	12450 3000 12625 3000
Text GLabel 12625 3000 2    50   Output ~ 0
IRSTR
Wire Wire Line
	12450 3100 12625 3100
Text GLabel 12625 3100 2    50   Output ~ 0
IWSTR
Wire Wire Line
	12450 3200 12625 3200
Text GLabel 12625 3200 2    50   Output ~ 0
IDBY
Wire Wire Line
	12450 3300 12625 3300
Text GLabel 12625 3300 2    50   Output ~ 0
ISPEED
Wire Wire Line
	12450 3400 12625 3400
Text GLabel 12625 3400 2    50   Output ~ 0
ICER
Wire Wire Line
	12450 3500 12625 3500
Text GLabel 12625 3500 2    50   Output ~ 0
IONL
Wire Wire Line
	12450 3600 12625 3600
Text GLabel 12625 3600 2    50   Input ~ 0
ITDA1
Wire Wire Line
	12450 3700 12625 3700
Text GLabel 12625 3700 2    50   Input ~ 0
IFAD
Wire Wire Line
	12450 3800 12625 3800
Text GLabel 12625 3800 2    50   Input ~ 0
IDEN
Wire Wire Line
	9850 1350 10025 1350
Text GLabel 10025 1350 2    50   Output ~ 0
IFBY
Wire Wire Line
	9850 1450 10025 1450
Text GLabel 10025 1450 2    50   Input ~ 0
ILWD
Wire Wire Line
	9850 1550 10025 1550
Text GLabel 10025 1550 2    50   Input ~ 0
IW4
Wire Wire Line
	9850 1650 10025 1650
Text GLabel 10025 1650 2    50   Input ~ 0
IGO
Wire Wire Line
	9850 1750 10025 1750
Text GLabel 10025 1750 2    50   Input ~ 0
IW0
Wire Wire Line
	9850 1850 10025 1850
Text GLabel 10025 1850 2    50   Input ~ 0
IW1
Wire Wire Line
	9850 1950 10025 1950
Text GLabel 10025 1950 2    50   Input Italic 0
ISGL
Wire Wire Line
	9850 2050 10025 2050
Text GLabel 10025 2050 2    50   Input Italic 0
ILOL
Wire Wire Line
	9850 2150 10025 2150
Text GLabel 10025 2150 2    50   Input ~ 0
IREV
Wire Wire Line
	9850 2250 10025 2250
Text GLabel 10025 2250 2    50   Input ~ 0
IREW
Wire Wire Line
	9850 2350 10025 2350
Text GLabel 10025 2350 2    50   Input ~ 0
IWP
Wire Wire Line
	9850 2450 10025 2450
Text GLabel 10025 2450 2    50   Input ~ 0
IW7
Wire Wire Line
	9850 2550 10025 2550
Text GLabel 10025 2550 2    50   Input ~ 0
IW3
Wire Wire Line
	9850 2650 10025 2650
Text GLabel 10025 2650 2    50   Input ~ 0
IW6
Wire Wire Line
	9850 2750 10025 2750
Text GLabel 10025 2750 2    50   Input ~ 0
IW2
Wire Wire Line
	9850 2850 10025 2850
Text GLabel 10025 2850 2    50   Input ~ 0
IW5
Wire Wire Line
	9850 2950 10025 2950
Text GLabel 10025 2950 2    50   Input ~ 0
IWRT
Wire Wire Line
	9850 3050 10025 3050
Text GLabel 10025 3050 2    50   Input Italic 0
IRTH2
Wire Wire Line
	9850 3150 10025 3150
Text GLabel 10025 3150 2    50   Input ~ 0
IEDIT
Wire Wire Line
	9850 3250 10025 3250
Text GLabel 10025 3250 2    50   Input ~ 0
IERASE
Wire Wire Line
	9850 3350 10025 3350
Text GLabel 10025 3350 2    50   Input ~ 0
IWFM
Wire Wire Line
	9850 3450 10025 3450
Text GLabel 10025 3450 2    50   Input Italic 0
IRTH1
Wire Wire Line
	9850 3550 10025 3550
Text GLabel 10025 3550 2    50   Input ~ 0
ITAD0
Wire Wire Line
	9850 3650 10025 3650
Wire Wire Line
	9850 3750 10025 3750
Text GLabel 11600 1500 0    50   Output ~ 0
IR1
Text GLabel 11600 1400 0    50   Output ~ 0
IRP
Wire Wire Line
	11600 1400 11950 1400
Wire Wire Line
	11600 1500 11950 1500
Wire Wire Line
	7050 4350 7225 4350
Wire Wire Line
	7050 4450 7225 4450
Wire Wire Line
	6375 4350 6550 4350
Wire Wire Line
	6375 4450 6550 4450
Wire Wire Line
	7050 4550 7225 4550
Wire Wire Line
	7050 4650 7225 4650
Wire Wire Line
	7050 4750 7225 4750
Wire Wire Line
	7050 4850 7225 4850
Wire Wire Line
	7050 4950 7225 4950
Wire Wire Line
	7050 5050 7225 5050
Wire Wire Line
	7050 5150 7225 5150
Wire Wire Line
	7050 5250 7225 5250
Wire Wire Line
	7050 5350 7225 5350
Wire Wire Line
	7050 5450 7225 5450
Wire Wire Line
	7050 5550 7225 5550
Wire Wire Line
	7050 5650 7225 5650
Wire Wire Line
	7050 5750 7225 5750
Wire Wire Line
	7050 5850 7225 5850
Wire Wire Line
	7050 5950 7225 5950
Wire Wire Line
	7050 6050 7225 6050
Wire Wire Line
	6375 4550 6550 4550
Wire Wire Line
	6375 4650 6550 4650
Wire Wire Line
	6375 4750 6550 4750
Wire Wire Line
	6375 4850 6550 4850
Wire Wire Line
	6375 4950 6550 4950
Wire Wire Line
	6375 5050 6550 5050
Wire Wire Line
	6375 5150 6550 5150
Wire Wire Line
	6375 5250 6550 5250
Wire Wire Line
	6375 5350 6550 5350
Wire Wire Line
	6375 5450 6550 5450
Wire Wire Line
	6375 5550 6550 5550
Wire Wire Line
	6375 5650 6550 5650
Wire Wire Line
	6375 5750 6550 5750
Wire Wire Line
	6375 5850 6550 5850
Wire Wire Line
	6375 5950 6550 5950
Wire Wire Line
	6375 6050 6550 6050
Wire Wire Line
	6375 4350 6375 4175
Wire Wire Line
	7225 4175 7225 4350
$Comp
L power:+5V #PWR0106
U 1 1 625DBDD8
P 7400 4175
F 0 "#PWR0106" H 7400 4025 50  0001 C CNN
F 1 "+5V" H 7400 4315 50  0000 C CNN
F 2 "" H 7400 4175 50  0001 C CNN
F 3 "" H 7400 4175 50  0001 C CNN
	1    7400 4175
	1    0    0    -1  
$EndComp
Wire Wire Line
	7225 4175 7400 4175
Connection ~ 7225 4175
Wire Wire Line
	6375 6050 6375 6225
Wire Wire Line
	6375 6225 7225 6225
Wire Wire Line
	7225 6225 7225 6050
$Comp
L power:GNDREF #PWR0107
U 1 1 625F347E
P 7400 6225
F 0 "#PWR0107" H 7400 5975 50  0001 C CNN
F 1 "GNDREF" H 7400 6075 50  0000 C CNN
F 2 "" H 7400 6225 50  0001 C CNN
F 3 "" H 7400 6225 50  0001 C CNN
	1    7400 6225
	1    0    0    -1  
$EndComp
Wire Wire Line
	7225 6225 7400 6225
Connection ~ 7225 6225
Text Label 7050 5950 0    50   ~ 0
PB0
Text Label 7050 5850 0    50   ~ 0
PB2
Text Label 7050 5750 0    50   ~ 0
PL0
Text Label 7050 5650 0    50   ~ 0
PL2
Text Label 7050 5550 0    50   ~ 0
PL4
Text Label 7050 5450 0    50   ~ 0
PL6
Text Label 7050 5350 0    50   ~ 0
PG0
Text Label 7050 5250 0    50   ~ 0
PG2
Text Label 7050 5150 0    50   ~ 0
PC0
Text Label 7050 5050 0    50   ~ 0
PC2
Text Label 7050 4950 0    50   ~ 0
PC4
Text Label 7050 4850 0    50   ~ 0
PC6
Text Label 7050 4750 0    50   ~ 0
PA7
Text Label 7050 4650 0    50   ~ 0
PA5
Text Label 7050 4550 0    50   ~ 0
PA3
Text Label 7050 4450 0    50   ~ 0
PA1
Text Label 6400 5950 0    50   ~ 0
PB1
Text Label 6400 5850 0    50   ~ 0
PB3
Text Label 6400 5750 0    50   ~ 0
PL1
Text Label 6400 5650 0    50   ~ 0
PL3
Text Label 6400 5550 0    50   ~ 0
PL5
Text Label 6400 5450 0    50   ~ 0
PL7
Text Label 6400 5350 0    50   ~ 0
PG1
Text Label 6400 5250 0    50   ~ 0
PD7
Text Label 6400 5150 0    50   ~ 0
PC1
Text Label 6400 5050 0    50   ~ 0
PC3
Text Label 6400 4950 0    50   ~ 0
PC5
Text Label 6400 4850 0    50   ~ 0
PC7
Text Label 6400 4750 0    50   ~ 0
PA6
Text Label 6400 4650 0    50   ~ 0
PA4
Text Label 6400 4550 0    50   ~ 0
PA2
Text Label 6400 4450 0    50   ~ 0
PA0
$Comp
L Connector_Generic:Conn_01x08 J5
U 1 1 62627916
P 4050 5850
F 0 "J5" H 4050 6250 50  0000 C CNN
F 1 "Mates with ANALOG IN A8-A15" H 4050 5350 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 4050 5850 50  0001 C CNN
F 3 "~" H 4050 5850 50  0001 C CNN
	1    4050 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 5550 3850 5550
Wire Wire Line
	3675 5650 3850 5650
Wire Wire Line
	3675 5750 3850 5750
Wire Wire Line
	3675 5850 3850 5850
Wire Wire Line
	3675 5950 3850 5950
Wire Wire Line
	3675 6050 3850 6050
Wire Wire Line
	3675 6150 3850 6150
Wire Wire Line
	3675 6250 3850 6250
Text Label 3675 6250 0    50   ~ 0
PK7
Text Label 3675 6150 0    50   ~ 0
PK6
Text Label 3675 6050 0    50   ~ 0
PK5
Text Label 3675 5950 0    50   ~ 0
PK4
Text Label 3675 5850 0    50   ~ 0
PK3
Text Label 3675 5750 0    50   ~ 0
PK2
Text Label 3675 5650 0    50   ~ 0
PK1
Text Label 3675 5550 0    50   ~ 0
PK0
$Comp
L Connector_Generic:Conn_01x08 J3
U 1 1 62641D9F
P 4050 3775
F 0 "J3" H 4050 4175 50  0000 C CNN
F 1 "Mates with POWER" H 4050 3275 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 4050 3775 50  0001 C CNN
F 3 "~" H 4050 3775 50  0001 C CNN
	1    4050 3775
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 3675 3850 3675
Wire Wire Line
	3075 3875 3850 3875
Wire Wire Line
	3200 3775 3500 3775
Wire Wire Line
	3425 3975 3850 3975
Wire Wire Line
	3425 4075 3850 4075
Wire Wire Line
	3675 4175 3850 4175
Text Label 3675 3675 0    50   ~ 0
RESET
NoConn ~ 3850 3475
NoConn ~ 3675 3675
$Comp
L power:+5V #PWR0108
U 1 1 6267B5CD
P 3075 3575
F 0 "#PWR0108" H 3075 3425 50  0001 C CNN
F 1 "+5V" H 3075 3715 50  0000 C CNN
F 2 "" H 3075 3575 50  0001 C CNN
F 3 "" H 3075 3575 50  0001 C CNN
	1    3075 3575
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0109
U 1 1 626E0802
P 3200 3775
F 0 "#PWR0109" H 3200 3625 50  0001 C CNN
F 1 "+3V3" H 3200 3915 50  0000 C CNN
F 2 "" H 3200 3775 50  0001 C CNN
F 3 "" H 3200 3775 50  0001 C CNN
	1    3200 3775
	1    0    0    -1  
$EndComp
Text Label 3725 4175 0    50   ~ 0
VIN
NoConn ~ 3675 4175
$Comp
L power:GNDREF #PWR0110
U 1 1 626FDC38
P 3425 4100
F 0 "#PWR0110" H 3425 3850 50  0001 C CNN
F 1 "GNDREF" H 3425 3950 50  0000 C CNN
F 2 "" H 3425 4100 50  0001 C CNN
F 3 "" H 3425 4100 50  0001 C CNN
	1    3425 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 3975 3425 4075
Connection ~ 3425 4075
Wire Wire Line
	3425 4075 3425 4100
Wire Wire Line
	3075 3875 3075 3575
Connection ~ 3075 3575
$Comp
L Connector_Generic:Conn_01x08 J4
U 1 1 62764CCA
P 4050 4850
F 0 "J4" H 4050 5250 50  0000 C CNN
F 1 "Mates with ANALOG IN A0-A7" H 4050 4350 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 4050 4850 50  0001 C CNN
F 3 "~" H 4050 4850 50  0001 C CNN
	1    4050 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3675 4550 3850 4550
Wire Wire Line
	3675 4650 3850 4650
Wire Wire Line
	3675 4750 3850 4750
Wire Wire Line
	3675 4850 3850 4850
Wire Wire Line
	3675 4950 3850 4950
Wire Wire Line
	3675 5050 3850 5050
Wire Wire Line
	3675 5150 3850 5150
Wire Wire Line
	3675 5250 3850 5250
Text Label 3675 5250 0    50   ~ 0
PF7
Text Label 3675 5150 0    50   ~ 0
PF6
Text Label 3675 5050 0    50   ~ 0
PF5
Text Label 3675 4950 0    50   ~ 0
PF4
Text Label 3675 4850 0    50   ~ 0
PF3
Text Label 3675 4750 0    50   ~ 0
PF2
Text Label 3675 4650 0    50   ~ 0
PF1
Text Label 3675 4550 0    50   ~ 0
PF0
Wire Wire Line
	5125 3275 5300 3275
Wire Wire Line
	5125 3375 5300 3375
Wire Wire Line
	5125 3475 5300 3475
Wire Wire Line
	5125 3675 5300 3675
Wire Wire Line
	5125 3775 5300 3775
Wire Wire Line
	5125 3875 5300 3875
Wire Wire Line
	5125 3975 5300 3975
Text Label 5125 3975 0    50   ~ 0
PB4
Text Label 5125 3875 0    50   ~ 0
PB5
Text Label 5125 3775 0    50   ~ 0
PB6
Text Label 5125 3675 0    50   ~ 0
PB7
Text Label 5125 3475 0    50   ~ 0
AREF
Text Label 5125 3375 0    50   ~ 0
PD1
Text Label 5125 3275 0    50   ~ 0
PD0
Wire Wire Line
	5125 4075 5300 4075
Text Label 5125 4075 0    50   ~ 0
PH6
$Comp
L power:GNDREF #PWR0111
U 1 1 62809B6B
P 4525 3575
F 0 "#PWR0111" H 4525 3325 50  0001 C CNN
F 1 "GNDREF" H 4525 3425 50  0000 C CNN
F 2 "" H 4525 3575 50  0001 C CNN
F 3 "" H 4525 3575 50  0001 C CNN
	1    4525 3575
	1    0    0    -1  
$EndComp
Wire Wire Line
	4525 3575 5300 3575
$Comp
L Connector_Generic:Conn_01x08 J7
U 1 1 6283E64C
P 5500 4900
F 0 "J7" H 5500 5300 50  0000 C CNN
F 1 "Mates with PWM 0-7" H 5500 4400 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 5500 4900 50  0001 C CNN
F 3 "~" H 5500 4900 50  0001 C CNN
	1    5500 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5125 4600 5300 4600
Wire Wire Line
	5125 4700 5300 4700
Wire Wire Line
	5125 4800 5300 4800
Wire Wire Line
	5125 4900 5300 4900
Wire Wire Line
	5125 5000 5300 5000
Wire Wire Line
	5125 5100 5300 5100
Wire Wire Line
	5125 5200 5300 5200
Wire Wire Line
	5125 5300 5300 5300
Text Label 5125 5300 0    50   ~ 0
PE0
Text Label 5125 5200 0    50   ~ 0
PE1
Text Label 5125 5100 0    50   ~ 0
PE4
Text Label 5125 5000 0    50   ~ 0
PE5
Text Label 5125 4900 0    50   ~ 0
PG5
Text Label 5125 4800 0    50   ~ 0
PE3
Text Label 5125 4700 0    50   ~ 0
PH3
Text Label 5125 4600 0    50   ~ 0
PH4
$Comp
L Connector_Generic:Conn_01x08 J8
U 1 1 62852257
P 5500 5950
F 0 "J8" H 5500 6350 50  0000 C CNN
F 1 "Mates with COMMUNICATION 14-21" H 5500 5450 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 5500 5950 50  0001 C CNN
F 3 "~" H 5500 5950 50  0001 C CNN
	1    5500 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5125 5650 5300 5650
Wire Wire Line
	5125 5750 5300 5750
Wire Wire Line
	5125 5850 5300 5850
Wire Wire Line
	5125 5950 5300 5950
Wire Wire Line
	5125 6050 5300 6050
Wire Wire Line
	5125 6150 5300 6150
Wire Wire Line
	5125 6250 5300 6250
Wire Wire Line
	5125 6350 5300 6350
Text Label 5125 6350 0    50   ~ 0
PD0
Text Label 5125 6250 0    50   ~ 0
PD1
Text Label 5125 6150 0    50   ~ 0
PD2
Text Label 5125 6050 0    50   ~ 0
PD3
Text Label 5125 5950 0    50   ~ 0
PH0
Text Label 5125 5850 0    50   ~ 0
PH1
Text Label 5125 5750 0    50   ~ 0
PJ0
Text Label 5125 5650 0    50   ~ 0
PJ1
Text GLabel 7225 4550 2    50   Output ~ 0
IW4
Text GLabel 7225 4750 2    50   Output ~ 0
IW0
Text GLabel 6375 4750 0    50   Output ~ 0
IW1
Text GLabel 6375 4450 0    50   Output ~ 0
IW7
Text GLabel 6375 4650 0    50   Output ~ 0
IW3
Text GLabel 7225 4650 2    50   Output ~ 0
IW2
Text GLabel 6375 4550 0    50   Output ~ 0
IW5
Text GLabel 3675 5050 0    50   Input ~ 0
IR2
Text GLabel 3675 4950 0    50   Input ~ 0
IR3
Text GLabel 3675 5250 0    50   Input ~ 0
IR0
Text GLabel 3675 4850 0    50   Input ~ 0
IR4
Text GLabel 3675 4550 0    50   Input ~ 0
IR7
Text GLabel 3675 4650 0    50   Input ~ 0
IR6
Text GLabel 3675 4750 0    50   Input ~ 0
IR5
Text GLabel 3675 5150 0    50   Input ~ 0
IR1
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 630E7B86
P 3500 3575
F 0 "#FLG0101" H 3500 3650 50  0001 C CNN
F 1 "PWR_FLAG" H 3500 3725 50  0000 C CNN
F 2 "" H 3500 3575 50  0001 C CNN
F 3 "~" H 3500 3575 50  0001 C CNN
	1    3500 3575
	1    0    0    -1  
$EndComp
Connection ~ 3500 3575
Wire Wire Line
	3500 3575 3850 3575
Wire Wire Line
	3075 3575 3500 3575
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 630E9CEB
P 3275 4075
F 0 "#FLG0102" H 3275 4150 50  0001 C CNN
F 1 "PWR_FLAG" H 3275 4225 50  0000 C CNN
F 2 "" H 3275 4075 50  0001 C CNN
F 3 "~" H 3275 4075 50  0001 C CNN
	1    3275 4075
	1    0    0    -1  
$EndComp
Wire Wire Line
	3425 4075 3275 4075
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 63166A4F
P 3500 3775
F 0 "#FLG0103" H 3500 3850 50  0001 C CNN
F 1 "PWR_FLAG" H 3500 3925 50  0000 C CNN
F 2 "" H 3500 3775 50  0001 C CNN
F 3 "~" H 3500 3775 50  0001 C CNN
	1    3500 3775
	1    0    0    -1  
$EndComp
Connection ~ 3500 3775
Wire Wire Line
	3500 3775 3850 3775
Text GLabel 10025 3750 2    50   Output ~ 0
IR3
Text GLabel 10025 3650 2    50   Output ~ 0
IR2
Text GLabel 7225 4450 2    50   Output ~ 0
IW6
Text GLabel 3675 5550 0    50   Input ~ 0
IFBY
Text GLabel 6375 4850 0    50   Output ~ 0
ILWD
Text GLabel 7225 4850 2    50   Output ~ 0
IGO
Text GLabel 5125 3975 0    50   Output Italic 0
ISGL
Text GLabel 5125 3875 0    50   Output Italic 0
ILOL
Text GLabel 7225 4950 2    50   Output ~ 0
IREV
Text GLabel 6375 4950 0    50   Output ~ 0
IREW
Text GLabel 6375 5450 0    50   Output ~ 0
IWP
Text GLabel 6375 5050 0    50   Output ~ 0
IWRT
Text GLabel 5125 3775 0    50   Output Italic 0
IRTH2
Text GLabel 7225 5050 2    50   Output ~ 0
IEDIT
Text GLabel 6375 5150 0    50   Output ~ 0
IERASE
Text GLabel 7225 5150 2    50   Output ~ 0
IWFM
Text GLabel 5125 3675 0    50   Output Italic 0
IRTH1
Text GLabel 7225 5450 2    50   Output ~ 0
ITAD0
Text GLabel 5125 6150 0    50   Input ~ 0
ICER
Text GLabel 5125 3375 0    50   Input ~ 0
IFMK
Text GLabel 7225 5250 2    50   Input ~ 0
IDENT
Text GLabel 6375 5550 0    50   Output ~ 0
IFEN
Text GLabel 3675 5750 0    50   Input ~ 0
IEOT
Text GLabel 7225 5550 2    50   Output ~ 0
IOFL
Text GLabel 7225 5350 2    50   Input ~ 0
INRZ
Text GLabel 3675 5850 0    50   Input ~ 0
IRDY
Text GLabel 3675 5950 0    50   Input ~ 0
IRWD
Text GLabel 3675 6050 0    50   Input ~ 0
IFPT
Text GLabel 5125 5000 0    50   Input ~ 0
IRSTR
Text GLabel 5125 5100 0    50   Input ~ 0
IWSTR
Text GLabel 5125 6050 0    50   Input ~ 0
IDBY
Text GLabel 3675 6150 0    50   Input ~ 0
ISPEED
Text GLabel 5125 3275 0    50   Input ~ 0
IHER
Text GLabel 3675 6250 0    50   Input ~ 0
IONL
Text GLabel 6375 5650 0    50   Output ~ 0
ITDA1
Text GLabel 7225 5650 2    50   Output ~ 0
IFAD
Text GLabel 6375 5750 0    50   Output ~ 0
IDEN
Text GLabel 6375 5350 0    50   Input ~ 0
IRP
Wire Wire Line
	5125 4175 5300 4175
Text Label 5125 4175 0    50   ~ 0
PH5
NoConn ~ 5125 5300
NoConn ~ 5125 5200
NoConn ~ 5125 4800
NoConn ~ 5125 5750
NoConn ~ 5125 5650
NoConn ~ 5125 5850
NoConn ~ 5125 5950
Text GLabel 3675 5650 0    50   Input ~ 0
ILDP
NoConn ~ 5125 4600
NoConn ~ 5125 4700
NoConn ~ 5125 4900
NoConn ~ 5125 4175
NoConn ~ 5125 4075
NoConn ~ 5125 3475
$Comp
L Connector_Generic:Conn_01x08 J10
U 1 1 62DE8B71
P 6650 1975
F 0 "J10" H 7109 1967 50  0000 C CNN
F 1 "SPI flash connector" H 7109 1876 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 6650 1975 50  0001 C CNN
F 3 "~" H 6650 1975 50  0001 C CNN
	1    6650 1975
	1    0    0    -1  
$EndComp
Wire Notes Line
	7950 7025 7950 2925
Wire Notes Line
	7950 2925 2575 2925
Wire Notes Line
	2575 2925 2575 7025
Text Notes 3375 6850 0    100  ~ 0
Male connectors on bottom of board \nto mate with Arduino Mega 2560 R3 sockets.\n
$Comp
L power:+5V #PWR03
U 1 1 62EB6864
P 6275 1675
F 0 "#PWR03" H 6275 1525 50  0001 C CNN
F 1 "+5V" H 6275 1815 50  0000 C CNN
F 2 "" H 6275 1675 50  0001 C CNN
F 3 "" H 6275 1675 50  0001 C CNN
	1    6275 1675
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6275 1675 6450 1675
Wire Wire Line
	5725 1875 6450 1875
$Comp
L power:GNDREF #PWR02
U 1 1 6305F3E2
P 5725 1875
F 0 "#PWR02" H 5725 1625 50  0001 C CNN
F 1 "GNDREF" H 5725 1725 50  0000 C CNN
F 2 "" H 5725 1875 50  0001 C CNN
F 3 "" H 5725 1875 50  0001 C CNN
	1    5725 1875
	1    0    0    -1  
$EndComp
Wire Wire Line
	6275 1975 6450 1975
Wire Wire Line
	6275 1775 6450 1775
NoConn ~ 6275 1775
Text GLabel 6275 1975 0    50   Input ~ 0
SPI_CLK
Wire Wire Line
	6275 2075 6450 2075
Text GLabel 6275 2075 0    50   Output ~ 0
SPI_CIPO
Wire Wire Line
	6275 2175 6450 2175
Text GLabel 6275 2175 0    50   Input ~ 0
SPI_COPI
Wire Wire Line
	6275 2275 6450 2275
Text GLabel 6275 2275 0    50   Input ~ 0
SPI_CS
Wire Wire Line
	5350 2600 5525 2600
Text GLabel 5350 2600 0    50   Output ~ 0
SPI_CD
Connection ~ 5525 2600
Wire Wire Line
	5525 2600 5900 2600
$Comp
L power:+5V #PWR01
U 1 1 6314B826
P 5525 2300
F 0 "#PWR01" H 5525 2150 50  0001 C CNN
F 1 "+5V" H 5525 2440 50  0000 C CNN
F 2 "" H 5525 2300 50  0001 C CNN
F 3 "" H 5525 2300 50  0001 C CNN
	1    5525 2300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5900 2600 5900 2375
Wire Wire Line
	5900 2375 6450 2375
Text GLabel 7225 5950 2    50   Output ~ 0
SPI_CS
Text GLabel 6375 5950 0    50   Output ~ 0
SPI_CLK
Text GLabel 7225 5850 2    50   Output ~ 0
SPI_COPI
Text GLabel 6375 5850 0    50   Input ~ 0
SPI_CIPO
Text GLabel 6375 5250 0    50   Input ~ 0
SPI_CD
$Comp
L Device:R_US R1
U 1 1 6314A4CD
P 5525 2450
F 0 "R1" H 5690 2496 50  0000 C CNN
F 1 "10 kΩ" H 5690 2405 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 5565 2440 50  0001 C CNN
F 3 "~" H 5525 2450 50  0001 C CNN
	1    5525 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6375 4175 7225 4175
NoConn ~ 7225 5750
Wire Notes Line
	2600 7025 7950 7025
$Comp
L Connector_Generic:Conn_01x10 J6
U 1 1 627E66AB
P 5500 3675
F 0 "J6" H 5500 4175 50  0000 C CNN
F 1 "Mates with JP6 ANALOG IN A8-A13+others" H 5500 3075 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 5500 3675 50  0001 C CNN
F 3 "~" H 5500 3675 50  0001 C CNN
	1    5500 3675
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x18_Odd_Even J9
U 1 1 6257E05E
P 6750 5150
F 0 "J9" H 6800 6050 50  0000 C CNN
F 1 "Mates with XIO DIGITAL 22-53" H 6800 4150 50  0000 C CNN
F 2 "pertec_arduino_adapter:PinHeader_2x18_P2.54mm_Vertical_Mirror" H 6750 5150 50  0001 C CNN
F 3 "~" H 6750 5150 50  0001 C CNN
	1    6750 5150
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 2 1 63483023
P 8650 4825
F 0 "RN3" V 8500 4815 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 8420 4815 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 8200 4875 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 8630 4865 50  0001 C CNN
	2    8650 4825
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 4 1 63483B8C
P 9650 4825
F 0 "RN3" V 9500 4815 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 9420 4815 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 9200 4875 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 9630 4865 50  0001 C CNN
	4    9650 4825
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 6 1 63484225
P 10625 4850
F 0 "RN3" V 10475 4840 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 10395 4840 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 10175 4900 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 10605 4890 50  0001 C CNN
	6    10625 4850
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 8 1 634848EC
P 11650 4825
F 0 "RN3" V 11500 4815 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 11420 4815 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 11200 4875 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 11630 4865 50  0001 C CNN
	8    11650 4825
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 1 1 6348523E
P 12700 4850
F 0 "RN4" V 12550 4840 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 12470 4840 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 12250 4900 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 12680 4890 50  0001 C CNN
	1    12700 4850
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 3 1 63485D15
P 13650 4825
F 0 "RN4" V 13500 4815 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 13420 4815 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 13200 4875 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 13630 4865 50  0001 C CNN
	3    13650 4825
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 5 1 63486466
P 14625 4850
F 0 "RN4" V 14475 4840 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 14395 4840 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 14175 4900 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 14605 4890 50  0001 C CNN
	5    14625 4850
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 7 1 634870F8
P 15375 4800
F 0 "RN4" V 15225 4790 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 15145 4790 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 14925 4850 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 15355 4840 50  0001 C CNN
	7    15375 4800
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 1 1 6348C85F
P 8550 6000
F 0 "RN3" V 8400 5990 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 8320 5990 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 8100 6050 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 8530 6040 50  0001 C CNN
	1    8550 6000
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 3 1 6348C865
P 9550 6025
F 0 "RN3" V 9400 6015 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 9320 6015 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 9100 6075 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 9530 6065 50  0001 C CNN
	3    9550 6025
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 5 1 6348C86B
P 10550 6100
F 0 "RN3" V 10400 6090 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 10320 6090 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 10100 6150 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 10530 6140 50  0001 C CNN
	5    10550 6100
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN3
U 7 1 6348C871
P 11550 6050
F 0 "RN3" V 11400 6040 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 11320 6040 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 11100 6100 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 11530 6090 50  0001 C CNN
	7    11550 6050
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 2 1 6348C877
P 12725 6150
F 0 "RN4" V 12575 6140 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 12495 6140 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 12275 6200 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 12705 6190 50  0001 C CNN
	2    12725 6150
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 4 1 6348C87D
P 13700 6200
F 0 "RN4" V 13550 6190 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 13470 6190 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 13250 6250 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 13680 6240 50  0001 C CNN
	4    13700 6200
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 6 1 6348C883
P 14700 6225
F 0 "RN4" V 14550 6215 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 14470 6215 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 14250 6275 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 14680 6265 50  0001 C CNN
	6    14700 6225
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network08_Split RN4
U 8 1 6348C889
P 15725 6225
F 0 "RN4" V 15575 6215 50  0000 C CNN
F 1 "R_Divider_Network08_Split" V 15495 6215 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP10" V 15275 6275 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 15705 6265 50  0001 C CNN
	8    15725 6225
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network06_Split RN2
U 1 1 634A85C9
P 8425 7225
F 0 "RN2" V 8275 7215 50  0000 C CNN
F 1 "R_Divider_Network06_Split" V 8195 7215 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP8" V 7975 7275 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 8405 7265 50  0001 C CNN
	1    8425 7225
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network06_Split RN2
U 2 1 634A8BAA
P 9450 7250
F 0 "RN2" V 9300 7240 50  0000 C CNN
F 1 "R_Divider_Network06_Split" V 9220 7240 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP8" V 9000 7300 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 9430 7290 50  0001 C CNN
	2    9450 7250
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network06_Split RN2
U 3 1 634A950B
P 10475 7250
F 0 "RN2" V 10325 7240 50  0000 C CNN
F 1 "R_Divider_Network06_Split" V 10245 7240 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP8" V 10025 7300 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 10455 7290 50  0001 C CNN
	3    10475 7250
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network06_Split RN2
U 4 1 634A9E96
P 11550 7275
F 0 "RN2" V 11400 7265 50  0000 C CNN
F 1 "R_Divider_Network06_Split" V 11320 7265 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP8" V 11100 7325 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 11530 7315 50  0001 C CNN
	4    11550 7275
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network06_Split RN2
U 5 1 634AA9CD
P 12675 7300
F 0 "RN2" V 12525 7290 50  0000 C CNN
F 1 "R_Divider_Network06_Split" V 12445 7290 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP8" V 12225 7350 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 12655 7340 50  0001 C CNN
	5    12675 7300
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network06_Split RN2
U 6 1 634AB477
P 13775 7375
F 0 "RN2" V 13625 7365 50  0000 C CNN
F 1 "R_Divider_Network06_Split" V 13545 7365 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP8" V 13325 7425 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 13755 7415 50  0001 C CNN
	6    13775 7375
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network04_Split RN1
U 1 1 634AC1EA
P 8350 8575
F 0 "RN1" V 8200 8565 50  0000 C CNN
F 1 "R_Divider_Network04_Split" V 8120 8565 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP6" V 7900 8625 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 8330 8615 50  0001 C CNN
	1    8350 8575
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network04_Split RN1
U 2 1 634ACC64
P 9400 8550
F 0 "RN1" V 9250 8540 50  0000 C CNN
F 1 "R_Divider_Network04_Split" V 9170 8540 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP6" V 8950 8600 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 9380 8590 50  0001 C CNN
	2    9400 8550
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network04_Split RN1
U 3 1 634AD986
P 10525 8550
F 0 "RN1" V 10375 8540 50  0000 C CNN
F 1 "R_Divider_Network04_Split" V 10295 8540 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP6" V 10075 8600 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 10505 8590 50  0001 C CNN
	3    10525 8550
	1    0    0    -1  
$EndComp
$Comp
L pertec_arduino_adapter:R_Divider_Network04_Split RN1
U 4 1 634AEA56
P 11650 8650
F 0 "RN1" V 11500 8640 50  0000 C CNN
F 1 "R_Divider_Network04_Split" V 11420 8640 50  0000 C CNN
F 2 "Resistor_THT:R_Array_SIP6" V 11200 8700 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4600x.pdf" H 11630 8690 50  0001 C CNN
	4    11650 8650
	1    0    0    -1  
$EndComp
Text GLabel 15875 6225 2    50   Output ~ 0
IFBY
Text GLabel 13925 7375 2    50   Output ~ 0
IR2
Text GLabel 8500 8575 2    50   Output ~ 0
IR3
Text GLabel 14850 6225 2    50   Input Italic 0
ISGL
Text GLabel 9600 7250 2    50   Output ~ 0
IRP
Text GLabel 13850 6200 2    50   Output ~ 0
IR1
Text GLabel 10700 6100 2    50   Output ~ 0
IR0
Text GLabel 8700 6000 2    50   Output ~ 0
ILDP
Text GLabel 12850 4850 2    50   Output ~ 0
IR4
Text GLabel 11800 4825 2    50   Output ~ 0
IR7
Text GLabel 10775 4850 2    50   Output ~ 0
IR6
Text GLabel 8800 4825 2    50   Output ~ 0
IHER
Text GLabel 9800 4825 2    50   Output ~ 0
IFMK
Text GLabel 13800 4825 2    50   Output ~ 0
IDENT
Text GLabel 14775 4850 2    50   Output ~ 0
IR5
Text GLabel 15525 4800 2    50   Output ~ 0
IEOT
Text GLabel 9700 6025 2    50   Output ~ 0
INRZ
Text GLabel 12875 6150 2    50   Output ~ 0
IRDY
Text GLabel 8575 7225 2    50   Output ~ 0
IRWD
Text GLabel 11700 6050 2    50   Output ~ 0
IFPT
Text GLabel 10625 7250 2    50   Output ~ 0
IRSTR
Text GLabel 11700 7275 2    50   Output ~ 0
IWSTR
Text GLabel 12825 7300 2    50   Output ~ 0
IDBY
Text GLabel 9550 8550 2    50   Output ~ 0
ISPEED
Text GLabel 10675 8550 2    50   Output ~ 0
ICER
Text GLabel 11800 8650 2    50   Output ~ 0
IONL
$Comp
L power:GNDREF #PWR0112
U 1 1 637F2065
P 8175 4225
F 0 "#PWR0112" H 8175 3975 50  0001 C CNN
F 1 "GNDREF" H 8175 4075 50  0000 C CNN
F 2 "" H 8175 4225 50  0001 C CNN
F 3 "" H 8175 4225 50  0001 C CNN
	1    8175 4225
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0113
U 1 1 637F33EE
P 8175 3725
F 0 "#PWR0113" H 8175 3575 50  0001 C CNN
F 1 "+5V" H 8175 3865 50  0000 C CNN
F 2 "" H 8175 3725 50  0001 C CNN
F 3 "" H 8175 3725 50  0001 C CNN
	1    8175 3725
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0114
U 1 1 6384A708
P 8650 5075
F 0 "#PWR0114" H 8650 4825 50  0001 C CNN
F 1 "GNDREF" H 8650 4925 50  0000 C CNN
F 2 "" H 8650 5075 50  0001 C CNN
F 3 "" H 8650 5075 50  0001 C CNN
	1    8650 5075
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0115
U 1 1 6384A70E
P 8650 4575
F 0 "#PWR0115" H 8650 4425 50  0001 C CNN
F 1 "+5V" H 8650 4715 50  0000 C CNN
F 2 "" H 8650 4575 50  0001 C CNN
F 3 "" H 8650 4575 50  0001 C CNN
	1    8650 4575
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0116
U 1 1 63865D71
P 9650 5075
F 0 "#PWR0116" H 9650 4825 50  0001 C CNN
F 1 "GNDREF" H 9650 4925 50  0000 C CNN
F 2 "" H 9650 5075 50  0001 C CNN
F 3 "" H 9650 5075 50  0001 C CNN
	1    9650 5075
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0117
U 1 1 63865D77
P 9650 4575
F 0 "#PWR0117" H 9650 4425 50  0001 C CNN
F 1 "+5V" H 9650 4715 50  0000 C CNN
F 2 "" H 9650 4575 50  0001 C CNN
F 3 "" H 9650 4575 50  0001 C CNN
	1    9650 4575
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0118
U 1 1 638812A0
P 10625 5100
F 0 "#PWR0118" H 10625 4850 50  0001 C CNN
F 1 "GNDREF" H 10625 4950 50  0000 C CNN
F 2 "" H 10625 5100 50  0001 C CNN
F 3 "" H 10625 5100 50  0001 C CNN
	1    10625 5100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0119
U 1 1 638812A6
P 10625 4600
F 0 "#PWR0119" H 10625 4450 50  0001 C CNN
F 1 "+5V" H 10625 4740 50  0000 C CNN
F 2 "" H 10625 4600 50  0001 C CNN
F 3 "" H 10625 4600 50  0001 C CNN
	1    10625 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0120
U 1 1 6389CA01
P 11650 5075
F 0 "#PWR0120" H 11650 4825 50  0001 C CNN
F 1 "GNDREF" H 11650 4925 50  0000 C CNN
F 2 "" H 11650 5075 50  0001 C CNN
F 3 "" H 11650 5075 50  0001 C CNN
	1    11650 5075
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0121
U 1 1 6389CA07
P 11650 4575
F 0 "#PWR0121" H 11650 4425 50  0001 C CNN
F 1 "+5V" H 11650 4715 50  0000 C CNN
F 2 "" H 11650 4575 50  0001 C CNN
F 3 "" H 11650 4575 50  0001 C CNN
	1    11650 4575
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0122
U 1 1 638B8086
P 12700 5100
F 0 "#PWR0122" H 12700 4850 50  0001 C CNN
F 1 "GNDREF" H 12700 4950 50  0000 C CNN
F 2 "" H 12700 5100 50  0001 C CNN
F 3 "" H 12700 5100 50  0001 C CNN
	1    12700 5100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0123
U 1 1 638B808C
P 12700 4600
F 0 "#PWR0123" H 12700 4450 50  0001 C CNN
F 1 "+5V" H 12700 4740 50  0000 C CNN
F 2 "" H 12700 4600 50  0001 C CNN
F 3 "" H 12700 4600 50  0001 C CNN
	1    12700 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0124
U 1 1 638D3913
P 13650 5075
F 0 "#PWR0124" H 13650 4825 50  0001 C CNN
F 1 "GNDREF" H 13650 4925 50  0000 C CNN
F 2 "" H 13650 5075 50  0001 C CNN
F 3 "" H 13650 5075 50  0001 C CNN
	1    13650 5075
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0125
U 1 1 638D3919
P 13650 4575
F 0 "#PWR0125" H 13650 4425 50  0001 C CNN
F 1 "+5V" H 13650 4715 50  0000 C CNN
F 2 "" H 13650 4575 50  0001 C CNN
F 3 "" H 13650 4575 50  0001 C CNN
	1    13650 4575
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0126
U 1 1 638EF01E
P 14625 5100
F 0 "#PWR0126" H 14625 4850 50  0001 C CNN
F 1 "GNDREF" H 14625 4950 50  0000 C CNN
F 2 "" H 14625 5100 50  0001 C CNN
F 3 "" H 14625 5100 50  0001 C CNN
	1    14625 5100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0127
U 1 1 638EF024
P 14625 4600
F 0 "#PWR0127" H 14625 4450 50  0001 C CNN
F 1 "+5V" H 14625 4740 50  0000 C CNN
F 2 "" H 14625 4600 50  0001 C CNN
F 3 "" H 14625 4600 50  0001 C CNN
	1    14625 4600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0128
U 1 1 6390A7A7
P 15375 5050
F 0 "#PWR0128" H 15375 4800 50  0001 C CNN
F 1 "GNDREF" H 15375 4900 50  0000 C CNN
F 2 "" H 15375 5050 50  0001 C CNN
F 3 "" H 15375 5050 50  0001 C CNN
	1    15375 5050
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0129
U 1 1 6390A7AD
P 15375 4550
F 0 "#PWR0129" H 15375 4400 50  0001 C CNN
F 1 "+5V" H 15375 4690 50  0000 C CNN
F 2 "" H 15375 4550 50  0001 C CNN
F 3 "" H 15375 4550 50  0001 C CNN
	1    15375 4550
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0130
U 1 1 6392613A
P 8550 6250
F 0 "#PWR0130" H 8550 6000 50  0001 C CNN
F 1 "GNDREF" H 8550 6100 50  0000 C CNN
F 2 "" H 8550 6250 50  0001 C CNN
F 3 "" H 8550 6250 50  0001 C CNN
	1    8550 6250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0131
U 1 1 63926140
P 8550 5750
F 0 "#PWR0131" H 8550 5600 50  0001 C CNN
F 1 "+5V" H 8550 5890 50  0000 C CNN
F 2 "" H 8550 5750 50  0001 C CNN
F 3 "" H 8550 5750 50  0001 C CNN
	1    8550 5750
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0132
U 1 1 6394177F
P 9550 6275
F 0 "#PWR0132" H 9550 6025 50  0001 C CNN
F 1 "GNDREF" H 9550 6125 50  0000 C CNN
F 2 "" H 9550 6275 50  0001 C CNN
F 3 "" H 9550 6275 50  0001 C CNN
	1    9550 6275
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0133
U 1 1 63941785
P 9550 5775
F 0 "#PWR0133" H 9550 5625 50  0001 C CNN
F 1 "+5V" H 9550 5915 50  0000 C CNN
F 2 "" H 9550 5775 50  0001 C CNN
F 3 "" H 9550 5775 50  0001 C CNN
	1    9550 5775
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0134
U 1 1 6395CD9A
P 10550 6350
F 0 "#PWR0134" H 10550 6100 50  0001 C CNN
F 1 "GNDREF" H 10550 6200 50  0000 C CNN
F 2 "" H 10550 6350 50  0001 C CNN
F 3 "" H 10550 6350 50  0001 C CNN
	1    10550 6350
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0135
U 1 1 6395CDA0
P 10550 5850
F 0 "#PWR0135" H 10550 5700 50  0001 C CNN
F 1 "+5V" H 10550 5990 50  0000 C CNN
F 2 "" H 10550 5850 50  0001 C CNN
F 3 "" H 10550 5850 50  0001 C CNN
	1    10550 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0136
U 1 1 6397847B
P 11550 6300
F 0 "#PWR0136" H 11550 6050 50  0001 C CNN
F 1 "GNDREF" H 11550 6150 50  0000 C CNN
F 2 "" H 11550 6300 50  0001 C CNN
F 3 "" H 11550 6300 50  0001 C CNN
	1    11550 6300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0137
U 1 1 63978481
P 11550 5800
F 0 "#PWR0137" H 11550 5650 50  0001 C CNN
F 1 "+5V" H 11550 5940 50  0000 C CNN
F 2 "" H 11550 5800 50  0001 C CNN
F 3 "" H 11550 5800 50  0001 C CNN
	1    11550 5800
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0138
U 1 1 63993A96
P 12725 6400
F 0 "#PWR0138" H 12725 6150 50  0001 C CNN
F 1 "GNDREF" H 12725 6250 50  0000 C CNN
F 2 "" H 12725 6400 50  0001 C CNN
F 3 "" H 12725 6400 50  0001 C CNN
	1    12725 6400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0139
U 1 1 63993A9C
P 12725 5900
F 0 "#PWR0139" H 12725 5750 50  0001 C CNN
F 1 "+5V" H 12725 6040 50  0000 C CNN
F 2 "" H 12725 5900 50  0001 C CNN
F 3 "" H 12725 5900 50  0001 C CNN
	1    12725 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0140
U 1 1 639AF339
P 13700 6450
F 0 "#PWR0140" H 13700 6200 50  0001 C CNN
F 1 "GNDREF" H 13700 6300 50  0000 C CNN
F 2 "" H 13700 6450 50  0001 C CNN
F 3 "" H 13700 6450 50  0001 C CNN
	1    13700 6450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0141
U 1 1 639AF33F
P 13700 5950
F 0 "#PWR0141" H 13700 5800 50  0001 C CNN
F 1 "+5V" H 13700 6090 50  0000 C CNN
F 2 "" H 13700 5950 50  0001 C CNN
F 3 "" H 13700 5950 50  0001 C CNN
	1    13700 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0142
U 1 1 639CADC8
P 14700 6475
F 0 "#PWR0142" H 14700 6225 50  0001 C CNN
F 1 "GNDREF" H 14700 6325 50  0000 C CNN
F 2 "" H 14700 6475 50  0001 C CNN
F 3 "" H 14700 6475 50  0001 C CNN
	1    14700 6475
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0143
U 1 1 639CADCE
P 14700 5975
F 0 "#PWR0143" H 14700 5825 50  0001 C CNN
F 1 "+5V" H 14700 6115 50  0000 C CNN
F 2 "" H 14700 5975 50  0001 C CNN
F 3 "" H 14700 5975 50  0001 C CNN
	1    14700 5975
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0144
U 1 1 639E65EF
P 15725 6475
F 0 "#PWR0144" H 15725 6225 50  0001 C CNN
F 1 "GNDREF" H 15725 6325 50  0000 C CNN
F 2 "" H 15725 6475 50  0001 C CNN
F 3 "" H 15725 6475 50  0001 C CNN
	1    15725 6475
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0145
U 1 1 639E65F5
P 15725 5975
F 0 "#PWR0145" H 15725 5825 50  0001 C CNN
F 1 "+5V" H 15725 6115 50  0000 C CNN
F 2 "" H 15725 5975 50  0001 C CNN
F 3 "" H 15725 5975 50  0001 C CNN
	1    15725 5975
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0146
U 1 1 63A01EE4
P 8425 7475
F 0 "#PWR0146" H 8425 7225 50  0001 C CNN
F 1 "GNDREF" H 8425 7325 50  0000 C CNN
F 2 "" H 8425 7475 50  0001 C CNN
F 3 "" H 8425 7475 50  0001 C CNN
	1    8425 7475
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0147
U 1 1 63A01EEA
P 8425 6975
F 0 "#PWR0147" H 8425 6825 50  0001 C CNN
F 1 "+5V" H 8425 7115 50  0000 C CNN
F 2 "" H 8425 6975 50  0001 C CNN
F 3 "" H 8425 6975 50  0001 C CNN
	1    8425 6975
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0148
U 1 1 63A1D531
P 9450 7500
F 0 "#PWR0148" H 9450 7250 50  0001 C CNN
F 1 "GNDREF" H 9450 7350 50  0000 C CNN
F 2 "" H 9450 7500 50  0001 C CNN
F 3 "" H 9450 7500 50  0001 C CNN
	1    9450 7500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0149
U 1 1 63A1D537
P 9450 7000
F 0 "#PWR0149" H 9450 6850 50  0001 C CNN
F 1 "+5V" H 9450 7140 50  0000 C CNN
F 2 "" H 9450 7000 50  0001 C CNN
F 3 "" H 9450 7000 50  0001 C CNN
	1    9450 7000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0150
U 1 1 63A38CD8
P 10475 7500
F 0 "#PWR0150" H 10475 7250 50  0001 C CNN
F 1 "GNDREF" H 10475 7350 50  0000 C CNN
F 2 "" H 10475 7500 50  0001 C CNN
F 3 "" H 10475 7500 50  0001 C CNN
	1    10475 7500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0151
U 1 1 63A38CDE
P 10475 7000
F 0 "#PWR0151" H 10475 6850 50  0001 C CNN
F 1 "+5V" H 10475 7140 50  0000 C CNN
F 2 "" H 10475 7000 50  0001 C CNN
F 3 "" H 10475 7000 50  0001 C CNN
	1    10475 7000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0152
U 1 1 63A544C7
P 11550 7525
F 0 "#PWR0152" H 11550 7275 50  0001 C CNN
F 1 "GNDREF" H 11550 7375 50  0000 C CNN
F 2 "" H 11550 7525 50  0001 C CNN
F 3 "" H 11550 7525 50  0001 C CNN
	1    11550 7525
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0153
U 1 1 63A544CD
P 11550 7025
F 0 "#PWR0153" H 11550 6875 50  0001 C CNN
F 1 "+5V" H 11550 7165 50  0000 C CNN
F 2 "" H 11550 7025 50  0001 C CNN
F 3 "" H 11550 7025 50  0001 C CNN
	1    11550 7025
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0154
U 1 1 63A6FD50
P 12675 7550
F 0 "#PWR0154" H 12675 7300 50  0001 C CNN
F 1 "GNDREF" H 12675 7400 50  0000 C CNN
F 2 "" H 12675 7550 50  0001 C CNN
F 3 "" H 12675 7550 50  0001 C CNN
	1    12675 7550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0155
U 1 1 63A6FD56
P 12675 7050
F 0 "#PWR0155" H 12675 6900 50  0001 C CNN
F 1 "+5V" H 12675 7190 50  0000 C CNN
F 2 "" H 12675 7050 50  0001 C CNN
F 3 "" H 12675 7050 50  0001 C CNN
	1    12675 7050
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0156
U 1 1 63A8B571
P 13775 7625
F 0 "#PWR0156" H 13775 7375 50  0001 C CNN
F 1 "GNDREF" H 13775 7475 50  0000 C CNN
F 2 "" H 13775 7625 50  0001 C CNN
F 3 "" H 13775 7625 50  0001 C CNN
	1    13775 7625
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0157
U 1 1 63A8B577
P 13775 7125
F 0 "#PWR0157" H 13775 6975 50  0001 C CNN
F 1 "+5V" H 13775 7265 50  0000 C CNN
F 2 "" H 13775 7125 50  0001 C CNN
F 3 "" H 13775 7125 50  0001 C CNN
	1    13775 7125
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0158
U 1 1 63AA6E60
P 8350 8825
F 0 "#PWR0158" H 8350 8575 50  0001 C CNN
F 1 "GNDREF" H 8350 8675 50  0000 C CNN
F 2 "" H 8350 8825 50  0001 C CNN
F 3 "" H 8350 8825 50  0001 C CNN
	1    8350 8825
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0159
U 1 1 63AA6E66
P 8350 8325
F 0 "#PWR0159" H 8350 8175 50  0001 C CNN
F 1 "+5V" H 8350 8465 50  0000 C CNN
F 2 "" H 8350 8325 50  0001 C CNN
F 3 "" H 8350 8325 50  0001 C CNN
	1    8350 8325
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0160
U 1 1 63AC26C5
P 9400 8800
F 0 "#PWR0160" H 9400 8550 50  0001 C CNN
F 1 "GNDREF" H 9400 8650 50  0000 C CNN
F 2 "" H 9400 8800 50  0001 C CNN
F 3 "" H 9400 8800 50  0001 C CNN
	1    9400 8800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0161
U 1 1 63AC26CB
P 9400 8300
F 0 "#PWR0161" H 9400 8150 50  0001 C CNN
F 1 "+5V" H 9400 8440 50  0000 C CNN
F 2 "" H 9400 8300 50  0001 C CNN
F 3 "" H 9400 8300 50  0001 C CNN
	1    9400 8300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0162
U 1 1 63ADE100
P 10525 8800
F 0 "#PWR0162" H 10525 8550 50  0001 C CNN
F 1 "GNDREF" H 10525 8650 50  0000 C CNN
F 2 "" H 10525 8800 50  0001 C CNN
F 3 "" H 10525 8800 50  0001 C CNN
	1    10525 8800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0163
U 1 1 63ADE106
P 10525 8300
F 0 "#PWR0163" H 10525 8150 50  0001 C CNN
F 1 "+5V" H 10525 8440 50  0000 C CNN
F 2 "" H 10525 8300 50  0001 C CNN
F 3 "" H 10525 8300 50  0001 C CNN
	1    10525 8300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDREF #PWR0164
U 1 1 63AF9975
P 11650 8900
F 0 "#PWR0164" H 11650 8650 50  0001 C CNN
F 1 "GNDREF" H 11650 8750 50  0000 C CNN
F 2 "" H 11650 8900 50  0001 C CNN
F 3 "" H 11650 8900 50  0001 C CNN
	1    11650 8900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0165
U 1 1 63AF997B
P 11650 8400
F 0 "#PWR0165" H 11650 8250 50  0001 C CNN
F 1 "+5V" H 11650 8540 50  0000 C CNN
F 2 "" H 11650 8400 50  0001 C CNN
F 3 "" H 11650 8400 50  0001 C CNN
	1    11650 8400
	1    0    0    -1  
$EndComp
$EndSCHEMATC
