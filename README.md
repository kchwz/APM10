Arduino driver for the AOSONG ASAIR APM10 Particulate Sensor.

## Features

[The APM10 sensor from ASAIR](http://www.aosong.com/m/en/products-76.html#:~:text=APM10%2DLaser%20Particle%20Detection%20Sensor,Aosong%20Electronic%20Co.%2C%20Ltd.&text=APM10%20is%20a%20digital%20particulate,0.3%20%CE%BCm%20to%2010%20%CE%BCm.) measures particulate matter from 0.3 to 10 microns with laser scattering.

The sensor has three modes: I2C, UART, and PWM. Examples are included for each mode.

**All of these modes are 5V logic, so make sure your chip GPIO pins are 5V tolerant! If not, you will need a logic level converter.** 

The library also includes functions to convert PM2.5 and PM10 concentration to the US Environmental Protection Agency's Air Quality Index (AQI) metric.

The driver and examples are compatible with any I2C, UART, or interrupt-capable Arduino microcontrollers.

## Download and Installation

Simply download the library .ZIP and extract the APM10 folder to your Arduino/libraries folder.