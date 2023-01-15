/*******************************************************************

  @file   APM10.cpp
  @author kchwz

  @section Introduction

  Library for APM10 Particulate Sensor.

  @section History

  Ver1.0 - Initial release of library. 

  @section License

  BSD license, all text here must be included in any redistribution

*******************************************************************/

#include "APM10.h"

#define SAMPLE_DELAY 250

bool APM10::beginI2C(uint8_t i2caddr, TwoWire &wirePort){
  _i2cPort = &wirePort;
  _i2caddr = i2caddr;
  _i2cPort->begin();
  sensorMode = 0;
  return isConnected();
}

bool APM10::beginUART(SoftwareSerial &serPort){
  _serialPort = &serPort;
  _serialPort->begin(baudRate);
  sensorMode = 1;
  return isConnected();
}

bool APM10::isConnected(){ //ping APM10 to verify whether is connected
 if(!sensorMode){
    _i2cPort->beginTransmission(_i2caddr);
    if(_i2cPort->endTransmission()!=0) return false;
    else return true;
 }
 else{
    _serialPort->write(READ_PM_CMD,5);
    for(int i = 0; i < 4; i++){
      if(_serialPort->available()){
        if(_serialPort->read()==0xFE){
          while(_serialPort->available()>0) _serialPort->read();
          return true;
        }
        else return false;
      }
      delay(250);
    }
    return false;
 }
}

void APM10::startMeasurement(){ //send command to start the APM10 sensor, I2C/PWM ONLY
  if(!sensorMode){
    _i2cPort->beginTransmission(_i2caddr);
    _i2cPort->write(byte(0x00));
    _i2cPort->write(byte(0x10));
    _i2cPort->write(byte(0x05));
    _i2cPort->write(byte(0x00));
    _i2cPort->write(byte(0xF6));
    _i2cPort->endTransmission();
  }
}

bool APM10::collectData(pmData& data){ //send command to collect data, read result and store
  if(!sensorMode){
    _i2cPort->beginTransmission(_i2caddr);
    _i2cPort->write(byte(0x03));
    _i2cPort->write(byte(0x00));
    _i2cPort->write(byte(0x11));
    _i2cPort->endTransmission();
    _i2cPort->requestFrom(8,30);
    if(30 >= _i2cPort->available()){
      byte MSB,LSB;
      
      MSB = _i2cPort->read();
      LSB = _i2cPort->read();
      data._PM1_0 = ((MSB << 8) | LSB);
      _i2cPort->read();

      MSB = _i2cPort->read();
      LSB = _i2cPort->read();
      data._PM2_5 = ((MSB << 8) | LSB);

      for(int i = 0; i < 4; i++){
        _i2cPort->read();
      }
      
      MSB = _i2cPort->read();
      LSB = _i2cPort->read();
      data._PM10 = ((MSB << 8) | LSB);

      while(_i2cPort->available()){ //read remaining bytes from sensor, all reserved
        _i2cPort->read();
      }
    }
    return true;
  }
  else{
    _serialPort->write(READ_PM_CMD,5);
    int uartReturn[11] = {0};
    int checkSum = 0;
    delay(SAMPLE_DELAY);
    for(int i = 0; i < 11; i++){
      uartReturn[i] = _serialPort->read();
      if(i!=0 && i!= 10){
        checkSum += uartReturn[i]; //exclude frame head and checksum
      }
    }
    data._PM1_0 = uartReturn[4]*256+uartReturn[5];
    data._PM2_5 = uartReturn[6]*256+uartReturn[7];
    data._PM10 = uartReturn[8]*256+uartReturn[9];
    if(uartReturn[10] != checkSum){ //verify data integrity
      return false;
    }
    return true;
  }
}

void APM10::stopMeasurement(){ //send command to stop the APM10 sensor, I2C/PWM ONLY
  if(!sensorMode){
    _i2cPort->beginTransmission(_i2caddr);
    _i2cPort->write(byte(0x01));
    _i2cPort->write(byte(0x04));
    _i2cPort->endTransmission();
  }
}

int APM10::PM2_5_TO_AQI(int PM){ //convert PM2.5 concentration to AQI value (EPA defined)
  if(PM < 0) return -1; //reject invalid PM value
  
  int d = -1;
  for(int i = 0; i < 7; i++){
    d = i;
    if(PM <= BP_hi_2_5[i]){
      break;
    }
  }
  return (I_hi[d]-I_lo[d])/(BP_hi_2_5[d]-BP_lo_2_5[d])*(PM - BP_lo_2_5[d])+I_lo[d]+0.5; //Use AQI formula and add 0.5 to round to nearest int
}

int APM10::PM10_TO_AQI(int PM){ //convert PM10 concentration to AQI value (EPA defined)
  if(PM < 0) return -1; //reject invalid PM value
  int d = -1;
  for(int i = 0; i < 7; i++){
    d = i;
    if(PM <= BP_hi_10[i]){
      break;
    }
  }
  return (I_hi[d]-I_lo[d])/(BP_hi_10[d]-BP_lo_10[d])*(PM - BP_lo_10[d])+I_lo[d]+0.5;  //Use AQI formula and add 0.5 to round to nearest int
}
