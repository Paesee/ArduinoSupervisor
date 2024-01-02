#include "ConfigPID.h"

// Constants definition
const float ANALOG2VOLTAGE = 0.004887;
const float VOLTAGE2ANALOG = 20;
const int OFFSET = 0;
const int FS = 5000;
const float TS = 1e6 * (1.0 / FS);
// Variables definition for analog read
int meas = 0;
bool is_converted = true;
float voltage = 0;
// Variables definition for PID Voltage Control
GeneralPID pid;
int lixo1 = GeneralPID_init(&pid, 0.0002, 0, 0 ,0);
int lixo2 = pid.setLimits(&pid, 5, 0);
float ctrl_action = 0;
int pwm_ctrl_action = 0;

void ISR5kHz(void)
{
  //digitalWrite(6, HIGH);
  // ConfigADC: configure to read A4
  configADC(A4);
  //  Read defined pin
  if (is_converted)
  {
    bitSet(ADCSRA, ADSC);
    is_converted = false;
  }
  if (bit_is_clear(ADCSRA, ADSC))
  {
    meas = ADC;
    is_converted = true;
  }
  voltage = ANALOG2VOLTAGE * (meas - OFFSET);
  // Call PID Routine
  ctrl_action = pid.execute(&pid, voltage);
  pwm_ctrl_action = 20 * ctrl_action;
  if(emergency){
    OCR0B = 0;
  }else{
    OCR0B = pwm_ctrl_action;
  }
  
  //digitalWrite(6, LOW);
}

void configADC(int pin)
{
  ADCSRA = 0b10000110; // C1:: Enable ADC, pre-scaler = 128
  ADMUX = 0x40 | (pin - A0);
}

void configPWM(void)
{
  TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << WGM02) | (0 << CS02) | (1 << CS01) | (0 << CS00);
  OCR0A = 100;
  OCR0B = 0;
}