/************************************************************
PWM Example for the APM10 particulate matter (PM) sensor.
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

WARNING: The APM10's PWM is 5V logic level. Make sure your
microcontroller is 5V tolerant -- if not you will need to use
a logic level shifter!

Note: to switch modes between I2C, UART, and PWM, the sensor 
must be fully reset (powered off and back on again).
************************************************************/
#include <APM10.h>

#define PIN_SENSOR_PWM 2 //Set this to an interrupt-capable GPIO pin

APM10 sensor;
APM10::pmData data;
volatile long pwmPM = 0;
volatile long prev_time = 0;
volatile long currTime;

void riseISR(){
  #if defined(ARDUINO_AVR_UNO)
    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_PWM), fallISR, FALLING); //Uno-specific interrupt
  #else
    attachInterrupt(PIN_SENSOR_PWM, fallISR, FALLING);
  #endif
  prev_time = micros();
}

void fallISR(){
  #if defined(ARDUINO_AVR_UNO)
    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_PWM), riseISR, RISING); //Uno-specific interrupt
  #else
    attachInterrupt(PIN_SENSOR_PWM, riseISR, RISING);
  #endif
  pwmPM = ((micros()-prev_time)+10)/1000; //divide by 1000 to convert micros to millis (unit of pwmPM), add 10 to round up
}

void setup() {
  Serial.begin(115200);
  Serial.println("Reading PWM values!");
  pinMode(PIN_SENSOR_PWM,INPUT);
  #if defined(ARDUINO_AVR_UNO)
    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_PWM), riseISR, RISING); //Uno-specific interrupt
  #else
    attachInterrupt(PIN_SENSOR_PWM, riseISR, RISING);
  #endif
  currTime = millis();
}

void loop() {
  if(millis() - currTime > 1000){//sensor refresh rate is 1 Hz
    Serial.print("pwmPM_2.5: "+String(pwmPM)+"\t"); //print PM2.5 value calculated from PWM
    Serial.println("AQI_2.5: "+String(sensor.PM2_5_TO_AQI(pwmPM))); //convert PM2.5 to AQI
    currTime = millis();
  }
}
