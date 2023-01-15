/************************************************************
UART Example for the APM10 particulate matter (PM) sensor.
Continuously outputs particulate data at sampling rate of
sensor (1 Hz).

Connect your microcontroller to the sensor as follows:

APM10 Pin #      MCU
---------------------------
Pin 1   ------>  +5V
Pin 2   ------>  GND
Pin 3     Not connected
Pin 4   ------>  TX_PIN
Pin 5   ------>  RX_PIN
Pin 6     Not connected
Pin 7     Not connected
Pin 8     Not connected

WARNING: The APM10's UART is 5V logic level. Make sure your
microcontroller is 5V tolerant -- if not you will need to use
a logic level shifter!

Note: to switch modes between I2C, UART, and PWM, the sensor 
must be fully reset (powered off and back on again).
************************************************************/
#include <APM10.h>
#include <SoftwareSerial.h> //using software serial, install through library manager if not already installed
#define RX_PIN 7 //Change this to your desired RX Pin!
#define TX_PIN 8 //Change this to your desired TX Pin!

APM10 sensor;
APM10::pmData data;
SoftwareSerial serialAPM10(RX_PIN,TX_PIN);
volatile long sampleMillis;


void setup() {
  Serial.begin(115200);

  if(sensor.beginUART(serialAPM10)){ //begin UART communications with APM10
    Serial.println("APM10 Initialized!");
  }else{
    Serial.println("APM10 Init Failed. Check your wiring and make sure sensor has 5V power, and if changing modes, remember to power cycle the sensor!");
    while(1);
  }
  sensor.startMeasurement();
  sampleMillis = millis();
}

const byte READ_PM_CMD[5] = {0xFE,0xA5,0x00,0x01,0xA6};

void loop() {
  if(millis() - sampleMillis > 1000){ //sensor has 1 Hz sampling rate
    if(sensor.collectData(data)){
      Serial.print("PM_2_5:"+String(data._PM2_5)+"\t"); //PM 2.5 in ug/m3
      Serial.print("AQI_2_5:"+String(sensor.PM2_5_TO_AQI(data._PM2_5))+"\t"); //AQI calculated from PM2.5
      Serial.print("PM_10:"+String(data._PM10)+"\t"); //PM 10 in ug/m3
      Serial.print("AQI_10:"+String(sensor.PM10_TO_AQI(data._PM10))+"\t"); //AQI calculated from PM10
      Serial.println();
    }
    else{
      Serial.println("UART checksum error, check your wiring!");
    }
    sampleMillis = millis();
  }
}
