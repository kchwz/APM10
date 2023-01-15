/*******************************************************************

  @file   APM10.h
  
  Library for APM10 Particulate Sensor.

  BSD license, all text here must be included in any redistribution

*******************************************************************/

#pragma once
#include "Arduino.h"
#include "Wire.h"
#include "SoftwareSerial.h"

#define APM10_ADDRESS 8 //7-bit address

class APM10{
  public:

    struct pmData{
      uint16_t _PM1_0;
      uint16_t _PM2_5;
      uint16_t _PM10;
    };
    
    bool beginI2C(uint8_t i2caddr = APM10_ADDRESS, TwoWire &wirePort = Wire);
    bool beginUART(SoftwareSerial &serPort);
    bool isConnected();
    void startMeasurement();
    bool collectData(pmData& data);
    void stopMeasurement();
    int PM2_5_TO_AQI(int PM);
    int PM10_TO_AQI(int PM);
    
  private:
    TwoWire *_i2cPort;
    uint8_t _i2caddr;
    bool sensorMode; //0 = I2C, 1 = UART
    SoftwareSerial *_serialPort;
    const uint16_t baudRate = 1200;

    //constants for AQI calculation, from EPA document https://www.airnow.gov/sites/default/files/2020-05/aqi-technical-assistance-document-sept2018.pdf
    const float I_hi[7] = {50.0, 100.0, 150.0, 200.0, 300.0, 400.0, 500.0};
    const float I_lo[7] = {0.0, 51.0, 101.0, 151.0, 201.0, 301.0, 401.0};
    const float BP_hi_2_5[7] = {12.0, 35.4, 55.4, 150.4, 250.4, 350.4, 500.4};
    const float BP_lo_2_5[7] = {0.0, 12.1, 35.5, 55.5, 150.5, 250.5, 350.5};
    const float BP_hi_10[7] = {54.0, 154.0, 254.0, 354.0, 424.0, 504.0, 604.0};
    const float BP_lo_10[7] = {0.0, 55.0, 155.0, 255.0, 355.0, 425.0, 505.0}; 
    const byte READ_PM_CMD[5] = {0xFE,0xA5,0x00,0x01,0xA6};
};
