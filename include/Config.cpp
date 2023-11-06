#include "Config.h"

void ISR5kHz(void)
{
  //Read defined pin using configADC
  measurement = readAnalogic();
  // Call PID Routine
  ctrl_action = PID->execute(PID->self, measurement);
  // Map to fit in PWM register range
  pwm_ctrl_action = (float)pwm_factor * ctrl_action;
  OCR0A = pwm_ctrl_action;
}

void configADC(int pin)
{
  ADCSRA = 0b10000110; // C1:: Enable ADC, pre-scaler = 128
  ADMUX = 0x40 | (pin - A0);
}

float readAnalogic(void)
{
  bool is_converted = 0;
  float measurement = 0;

  if (is_converted)
  {
    bitSet(ADCSRA, ADSC);
    is_converted = false;
  }
  if (bit_is_clear(ADCSRA, ADSC))
  {
    measurement = ADC;
    is_converted = true;
  }
  return measurement;
}