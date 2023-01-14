/************************************************************
I2C Example for the APM10 particulate matter (PM) sensor.

WARNING: The APM10's I2C is 5V logic level. Make sure your
microcontroller is 5V tolerant -- if not you will need to use
a logic level shifter!
************************************************************/
#include <APM10.h>

APM10 sensor;
APM10::pmData data;

void setup() {
  Serial.begin(115200);

  if(sensor.begin(8)){ //begin I2C communications with APM10, address 0x08 (7-bit)
    Serial.println("APM10 Initialized!");
  }else{
    Serial.println("APM10 Init Failed. Check your wiring and make sure sensor has 5V power!");
    while(1); //loop forever
  }
  sensor.startMeasurement();
}

void loop() {
  sensor.collectData(data); //gather PM data
  Serial.print("PM_2_5:"+String(data._PM2_5)+"\t"); //PM 2.5 in ug/m3
  Serial.print("AQI_2_5:"+String(sensor.PM2_5_TO_AQI(data._PM2_5))+"\t"); //AQI calculated from PM2.5
  Serial.print("PM_10:"+String(data._PM10)+"\t"); //PM 10 in ug/m3
  Serial.print("AQI_10:"+String(sensor.PM10_TO_AQI(data._PM10))+"\t"); //AQI calculated from PM10
  Serial.println();
  delay(1000); //wait for new data, sensor has 1 Hz sampling rate
}
