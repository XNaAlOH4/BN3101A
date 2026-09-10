# Uroswap
A prototype to perform the swapping of irrigation source for nurses

#### Current Pinout plan will be as follows

P2,3 - Load cell 1

P4,5 - Load cell 2

P6,7 - Load cell 3

P9 - Servo 1

P10 - Servo 2

Timer configurations

Timer 1 used for Servos

Timer 2 set to 500Hz for regular weight monitoring

## Dependancies
- Servo
- HX711

### How to install

#### Command Line
```bash
arduino-cli lib install servo HX711
```

#### Graphical User Interface(GUI)
Follow [this tutorial](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library)
Search for both the Servo and HX711 libraries and install them


## Datasheet
[ATmega328P](https://docs.arduino.cc/resources/datasheets/Atmel-42735-8-bit-AVR-Microcontroller-ATmega328-328P_Datasheet.pdf)

### Notes
#### Interrupt vector table at section 16

#### Timer2 at section 22

22.7.2 Clear Timer on Compare Match (CTC) Mode

22.11 Register desc

