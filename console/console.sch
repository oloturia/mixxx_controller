EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "MIXXX CONSOLE"
Date "2024-01-09"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Leonardo ARDUINO1
U 1 1 65974FEC
P 5750 2600
F 0 "ARDUINO1" H 5750 3781 50  0000 C CNN
F 1 "Arduino_Leonardo" H 5750 3690 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 5750 2600 50  0001 C CIN
F 3 "https://www.arduino.cc/en/Main/ArduinoBoardLeonardo" H 5750 2600 50  0001 C CNN
	1    5750 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Left_BPM1
U 1 1 65976467
P 7000 2850
F 0 "RV_Left_BPM1" H 6931 2896 50  0000 R CNN
F 1 "R_POT" H 6931 2805 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTA4543_Single_Slide" H 7000 2850 50  0001 C CNN
F 3 "~" H 7000 2850 50  0001 C CNN
	1    7000 2850
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Left_Treble1
U 1 1 65976A6B
P 8000 3000
F 0 "RV_Left_Treble1" H 7931 3046 50  0000 R CNN
F 1 "R_POT" H 7931 2955 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTV09A-1_Single_Vertical" H 8000 3000 50  0001 C CNN
F 3 "~" H 8000 3000 50  0001 C CNN
	1    8000 3000
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Right_Treble1
U 1 1 65976F71
P 8850 3000
F 0 "RV_Right_Treble1" H 8781 3046 50  0000 R CNN
F 1 "R_POT" H 8781 2955 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTV09A-1_Single_Vertical" H 8850 3000 50  0001 C CNN
F 3 "~" H 8850 3000 50  0001 C CNN
	1    8850 3000
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Crossfader1
U 1 1 659775A2
P 7000 2350
F 0 "RV_Crossfader1" H 6931 2396 50  0000 R CNN
F 1 "R_POT" H 6931 2305 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTA6043_Single_Slide" H 7000 2350 50  0001 C CNN
F 3 "~" H 7000 2350 50  0001 C CNN
	1    7000 2350
	-1   0    0    1   
$EndComp
$Comp
L Device:R_POT RV_Left_Bass1
U 1 1 659779F0
P 8000 2000
F 0 "RV_Left_Bass1" H 7931 2046 50  0000 R CNN
F 1 "R_POT" H 7931 1955 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTV09A-1_Single_Vertical" H 8000 2000 50  0001 C CNN
F 3 "~" H 8000 2000 50  0001 C CNN
	1    8000 2000
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Left_Mid1
U 1 1 65977E3F
P 8000 2500
F 0 "RV_Left_Mid1" H 7931 2546 50  0000 R CNN
F 1 "R_POT" H 7931 2455 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTV09A-1_Single_Vertical" H 8000 2500 50  0001 C CNN
F 3 "~" H 8000 2500 50  0001 C CNN
	1    8000 2500
	-1   0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW_Left1
U 1 1 6597BE83
P 5350 4550
F 0 "SW_Left1" H 5350 4835 50  0000 C CNN
F 1 "SW_Push" H 5350 4744 50  0000 C CNN
F 2 "digikey-footprints:D6R90F2LFS" H 5350 4750 50  0001 C CNN
F 3 "~" H 5350 4750 50  0001 C CNN
	1    5350 4550
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW_Right1
U 1 1 6597C315
P 6000 4550
F 0 "SW_Right1" H 6000 4835 50  0000 C CNN
F 1 "SW_Push" H 6000 4744 50  0000 C CNN
F 2 "digikey-footprints:D6R90F2LFS" H 6000 4750 50  0001 C CNN
F 3 "~" H 6000 4750 50  0001 C CNN
	1    6000 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Right_Mid2
U 1 1 6597BAE6
P 8850 2500
F 0 "RV_Right_Mid2" H 8781 2546 50  0000 R CNN
F 1 "R_POT" H 8781 2455 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTV09A-1_Single_Vertical" H 8850 2500 50  0001 C CNN
F 3 "~" H 8850 2500 50  0001 C CNN
	1    8850 2500
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Right_Bass1
U 1 1 6597BAE0
P 8850 2000
F 0 "RV_Right_Bass1" H 8781 2046 50  0000 R CNN
F 1 "R_POT" H 8781 1955 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTV09A-1_Single_Vertical" H 8850 2000 50  0001 C CNN
F 3 "~" H 8850 2000 50  0001 C CNN
	1    8850 2000
	-1   0    0    -1  
$EndComp
$Comp
L Device:R_POT RV_Right_BPM1
U 1 1 6597BADA
P 7000 3350
F 0 "RV_Right_BPM1" H 6931 3396 50  0000 R CNN
F 1 "R_POT" H 6931 3305 50  0000 R CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_PTA4543_Single_Slide" H 7000 3350 50  0001 C CNN
F 3 "~" H 7000 3350 50  0001 C CNN
	1    7000 3350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6250 2600 6500 2600
Wire Wire Line
	6500 2600 6500 2350
Wire Wire Line
	6500 2350 6850 2350
Wire Wire Line
	6250 2700 6500 2700
Wire Wire Line
	6500 2700 6500 2850
Wire Wire Line
	6500 2850 6850 2850
Wire Wire Line
	6250 2800 6450 2800
Wire Wire Line
	6450 2800 6450 3350
Wire Wire Line
	6450 3350 6850 3350
Wire Wire Line
	5950 1600 7000 1600
Wire Wire Line
	7000 1600 7000 2200
Wire Wire Line
	7000 1600 7500 1600
Wire Wire Line
	7500 1600 7500 2700
Wire Wire Line
	7500 2700 7000 2700
Connection ~ 7000 1600
Wire Wire Line
	7500 2700 7500 3200
Wire Wire Line
	7500 3200 7000 3200
Connection ~ 7500 2700
Wire Wire Line
	7000 2500 6700 2500
Wire Wire Line
	6700 2500 6700 3000
Wire Wire Line
	6700 3700 5850 3700
Wire Wire Line
	7000 3000 6700 3000
Connection ~ 6700 3000
Wire Wire Line
	6700 3000 6700 3500
Wire Wire Line
	7000 3500 6700 3500
Connection ~ 6700 3500
Wire Wire Line
	6700 3500 6700 3700
Wire Wire Line
	7500 1600 8000 1600
Wire Wire Line
	8000 1600 8000 1850
Connection ~ 7500 1600
Connection ~ 8000 1850
Wire Wire Line
	8000 1850 7750 1850
Wire Wire Line
	7750 1850 7750 2350
Wire Wire Line
	7750 2350 8000 2350
Wire Wire Line
	7750 2350 7750 2850
Wire Wire Line
	7750 2850 8000 2850
Connection ~ 7750 2350
Wire Wire Line
	6700 3700 8000 3700
Wire Wire Line
	8000 3700 8000 3150
Connection ~ 6700 3700
Wire Wire Line
	8850 3150 9200 3150
Wire Wire Line
	9200 3150 9200 2650
Wire Wire Line
	9200 2650 8850 2650
Connection ~ 8850 3150
Connection ~ 8850 2650
Wire Wire Line
	9200 2650 9200 2150
Wire Wire Line
	9200 2150 8850 2150
Connection ~ 9200 2650
Connection ~ 8850 2150
Wire Wire Line
	6600 2900 6600 2000
Wire Wire Line
	6600 2000 7850 2000
Wire Wire Line
	6250 2900 6600 2900
Wire Wire Line
	6250 3000 6400 3000
Wire Wire Line
	6400 3000 6400 2150
Wire Wire Line
	6400 2150 7650 2150
Wire Wire Line
	7650 2150 7650 2500
Wire Wire Line
	7650 2500 7850 2500
Wire Wire Line
	6250 3100 7850 3100
Wire Wire Line
	7850 3100 7850 3000
Wire Wire Line
	8000 1850 8850 1850
Wire Wire Line
	8000 2150 8850 2150
Wire Wire Line
	8000 2350 8850 2350
Connection ~ 8000 2350
Wire Wire Line
	8000 2650 8850 2650
Wire Wire Line
	8000 2850 8850 2850
Connection ~ 8000 2850
Wire Wire Line
	8000 3150 8850 3150
Connection ~ 8000 3150
Wire Wire Line
	5100 1250 8700 1250
Wire Wire Line
	8700 1250 8700 2000
Wire Wire Line
	5250 2600 5050 2600
Wire Wire Line
	5050 2600 5050 1100
Wire Wire Line
	5050 1100 8650 1100
Wire Wire Line
	8650 1100 8650 2500
Wire Wire Line
	8650 2500 8700 2500
Wire Wire Line
	8700 4150 8700 3000
Wire Wire Line
	5050 4150 8700 4150
Wire Wire Line
	5100 1250 5100 3000
Wire Wire Line
	5100 3000 5250 3000
$Comp
L Device:Rotary_Encoder_Switch SW_Selector1
U 1 1 6597C931
P 4300 2000
F 0 "SW_Selector1" H 4300 2367 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 4300 2276 50  0000 C CNN
F 2 "digikey-footprints:XDCR_PEC11R-4215F-S0024" H 4150 2160 50  0001 C CNN
F 3 "~" H 4300 2260 50  0001 C CNN
	1    4300 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW_Left_Table_Control1
U 1 1 6597D2A5
P 4300 2800
F 0 "SW_Left_Table_Control1" H 4300 3167 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 4300 3076 50  0000 C CNN
F 2 "digikey-footprints:XDCR_PEC11R-4215F-S0024" H 4150 2960 50  0001 C CNN
F 3 "~" H 4300 3060 50  0001 C CNN
	1    4300 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4000 5850 3700
Connection ~ 5850 3700
Wire Wire Line
	5150 4000 5150 4550
Wire Wire Line
	5150 4000 5800 4000
Wire Wire Line
	5800 4000 5800 4550
Connection ~ 5800 4000
Wire Wire Line
	5800 4000 5850 4000
Wire Wire Line
	5050 4150 5050 3200
Wire Wire Line
	5050 3200 5250 3200
Wire Wire Line
	5250 3100 5200 3100
Wire Wire Line
	5200 3100 5200 3800
Wire Wire Line
	5200 3800 5550 3800
Wire Wire Line
	5550 3800 5550 4550
Wire Wire Line
	5250 3300 5250 3750
Wire Wire Line
	5250 3750 5600 3750
Wire Wire Line
	5600 3750 5600 3900
Wire Wire Line
	5600 3900 6200 3900
Wire Wire Line
	6200 3900 6200 4550
$Comp
L Device:Rotary_Encoder_Switch SW_Right_Table_Control1
U 1 1 6597DC98
P 4300 3650
F 0 "SW_Right_Table_Control1" H 4300 4017 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 4300 3926 50  0000 C CNN
F 2 "digikey-footprints:XDCR_PEC11R-4215F-S0024" H 4150 3810 50  0001 C CNN
F 3 "~" H 4300 3910 50  0001 C CNN
	1    4300 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 4000 4700 4000
Wire Wire Line
	4700 4000 4700 3750
Wire Wire Line
	4700 3750 4600 3750
Connection ~ 5150 4000
Wire Wire Line
	4700 3750 4700 2900
Wire Wire Line
	4700 2900 4600 2900
Connection ~ 4700 3750
Wire Wire Line
	4700 2900 4700 2100
Wire Wire Line
	4700 2100 4600 2100
Connection ~ 4700 2900
Wire Wire Line
	4600 2800 4600 2700
Wire Wire Line
	4600 2800 5250 2800
Wire Wire Line
	4750 2700 4750 1900
Wire Wire Line
	4750 1900 4600 1900
Wire Wire Line
	4750 2700 5250 2700
Wire Wire Line
	4750 2900 4750 3550
Wire Wire Line
	4750 3550 4600 3550
Wire Wire Line
	4750 2900 5250 2900
Wire Wire Line
	4700 4000 3500 4000
Wire Wire Line
	3500 4000 3500 3650
Wire Wire Line
	3500 3650 4000 3650
Connection ~ 4700 4000
Wire Wire Line
	3500 3650 3500 2800
Wire Wire Line
	3500 2800 4000 2800
Connection ~ 3500 3650
Wire Wire Line
	3500 2800 3500 2000
Wire Wire Line
	3500 2000 4000 2000
Connection ~ 3500 2800
Wire Wire Line
	4000 1900 3800 1900
Wire Wire Line
	3800 1900 3800 1400
Wire Wire Line
	3800 1400 5150 1400
Wire Wire Line
	5150 1400 5150 2000
Wire Wire Line
	5150 2000 5250 2000
Wire Wire Line
	5250 2200 4950 2200
Wire Wire Line
	4950 2200 4950 2300
Wire Wire Line
	4950 2300 3850 2300
Wire Wire Line
	3850 2300 3850 2700
Wire Wire Line
	3850 2700 4000 2700
Wire Wire Line
	5250 2400 3900 2400
Wire Wire Line
	3900 2400 3900 2900
Wire Wire Line
	3900 2900 4000 2900
Wire Wire Line
	5250 2300 5000 2300
Wire Wire Line
	5000 2300 5000 3250
Wire Wire Line
	5000 3250 3850 3250
Wire Wire Line
	3850 3250 3850 3550
Wire Wire Line
	3850 3550 4000 3550
Wire Wire Line
	5250 2500 4850 2500
Wire Wire Line
	4850 2500 4850 3900
Wire Wire Line
	4850 3900 4000 3900
Wire Wire Line
	4000 3900 4000 3750
Wire Wire Line
	4000 2100 3850 2100
Wire Wire Line
	3850 2100 3850 1500
Wire Wire Line
	3850 1500 5000 1500
Wire Wire Line
	5000 1500 5000 2100
Wire Wire Line
	5000 2100 5250 2100
$EndSCHEMATC
