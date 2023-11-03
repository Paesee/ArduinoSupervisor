// Libraries to be included
#include <Arduino.h>
#include "GeneralPID.h"
#include "TimerOne.h"

// Variables definition for analog read
float measurement = 0;
bool is_converted = 0;

// Variables definition for PID Voltage Control
GeneralPID_t *PID = GeneralPID_creator(1, 1, 1, 1, 1, 1);
const int fs = 5000;               // sampling frequency
const float ts = 1e6 * (1.0 / fs); // sampling time in microseconds
float ctrl_action = 0;
float pwm_factor = 1;
int pwm_ctrl_action = 0;

// Prototype for Interrupt Service Routine
void ISR_5kHz(void);

void setup(void)
{
  // Pin setup
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, INPUT);
  // MCU Registers configuration
  // ADC: setup registers in order to read without using AnalogRead()
  ADCSRA = 0b10000110; // C1:: Enable ADC, pre-scaler = 128
  ADMUX = 0b01000101;  // C2:: Use AVCC and A5 as input to ADC
  // PWM: setup registers to generate PWM signal on digital output [NUMBER]
  OCR0A = 0;                              // Duty cycle of 40%
  TCCR0A |= (1 << COM0A1) | (1 << WGM00); // Non-Inverting Phase Correct PWM mode 1
  TCCR0B |= (1 << CS00);                  // No-Prescalar
  // Serial
  Serial.begin(9600);
  // TimerOne: library used to configure registers for ISR
  Timer1.initialize(ts);
  Timer1.attachInterrupt(ISR_5kHz);
  // GeneralPID: library used to control capacitor voltage
  PID->setSetpoint(PID->self, 3.5);
}

void loop(void)
{
  Serial.print(PID->setpoint);
  Serial.print(";");
  Serial.print(PID->setpoint);
}

void ISR_5kHz(void)
{
  // Analog read
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
  // Call PID Routine
  ctrl_action = PID->execute(PID->self, measurement);
  // Map to fit in PWM register range
  pwm_ctrl_action = (float)pwm_factor * ctrl_action;
  OCR0A = pwm_ctrl_action;
}