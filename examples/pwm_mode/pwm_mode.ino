#include <APM10.h>

#define PIN_SENSOR_PWM 14 //Set this to an interrupt-capable GPIO pin

APM10 sensor;
APM10::pmData data;
volatile long pwmPM = 0;
volatile long prev_time = 0;

void riseISR(){
  attachInterrupt(PIN_SENSOR_PWM, fallISR, FALLING);
  prev_time = micros();
}

void fallISR(){
  attachInterrupt(PIN_SENSOR_PWM, riseISR, RISING);
  pwmPM = ((micros()-prev_time)+10)/1000; //divide by 1000 to convert micros to millis (unit of pwmPM), add 10 to round up
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_SENSOR_PWM,INPUT);
  attachInterrupt(PIN_SENSOR_PWM, riseISR, RISING); //interrupt for PWM timing
}

void loop() {
  Serial.print("pwmPM_2.5: "+String(pwmPM)+"\t"); //print PM2.5 value calculated from PWM
  Serial.println("APM10_2.5: "+String(sensor.PM2_5_TO_AQI(pwmPM))); //convert PM2.5 to AQI
  delay(1000); //sensor refresh rate is 1 Hz
}
