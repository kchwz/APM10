/************************************************************
I2C Example for the APM10 particulate matter (PM) sensor.
Takes samples for 10 seconds and then enters sleep mode.

Connect your microcontroller to the sensor as follows:

APM10 Pin #      MCU
---------------------------
Pin 1   ------>  +5V
Pin 2   ------>  GND
Pin 3   ------>  GND
Pin 4   ------>  SDA
Pin 5   ------>  SCL
Pin 6      Not connected
Pin 7      Not connected
Pin 8      Not connected

WARNING: The APM10's I2C is 5V logic level. Make sure your
microcontroller is 5V tolerant -- if not you will need to use
a logic level shifter!

Note: to switch modes between I2C and UART, the sensor must
be fully reset (powered off and back on again).
************************************************************/
#include <APM10.h>

APM10 sensor;
APM10::pmData data;
volatile long sleepMillis, sampleMillis;
bool sensorOn;

void setup() {
  Serial.begin(115200);

  if(sensor.beginI2C(8)){ //begin I2C communications with APM10, address 0x08 (7-bit)
    Serial.println("APM10 Initialized!");
  }else{
    Serial.println("APM10 Init Failed. Check your wiring and make sure sensor has 5V power, and if changing modes, remember to power cycle the sensor!");
    while(1); //loop forever
  }
  sensor.startMeasurement();
  sleepMillis = millis();
  sampleMillis = millis();
  sensorOn = true;
}

void loop() {
  if(millis() - sleepMillis < 10000){
    if(millis() - sampleMillis > 1000){ //sensor has 1 Hz sampling rate
      sensor.collectData(data); //gather PM data
      Serial.print("PM_2_5:"+String(data._PM2_5)+"\t"); //PM 2.5 in ug/m3
      Serial.print("AQI_2_5:"+String(sensor.PM2_5_TO_AQI(data._PM2_5))+"\t"); //AQI calculated from PM2.5
      Serial.print("PM_10:"+String(data._PM10)+"\t"); //PM 10 in ug/m3
      Serial.print("AQI_10:"+String(sensor.PM10_TO_AQI(data._PM10))+"\t"); //AQI calculated from PM10
      Serial.println();
      sampleMillis = millis();
    }
  }
  else if(sensorOn){
    Serial.println("Entering sleep mode.");
    sensor.stopMeasurement();
    sensorOn = false;
  }
}