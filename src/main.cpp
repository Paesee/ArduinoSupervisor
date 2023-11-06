// Include configuration file
#include <Config.h>

void setup(void)
{
  // Pin setup
  pinMode(13, OUTPUT);
  // MCU Registers configuration
  configADC(A5);
  // PWM: setup registers to generate PWM signal on digital output [NUMBER]
  OCR0A = 0;                              // Duty cycle of 40%
  TCCR0A |= (1 << COM0A1) | (1 << WGM00); // Non-Inverting Phase Correct PWM mode 1
  TCCR0B |= (1 << CS00);                  // No-Prescalar
  // Serial
  Serial.begin(9600);
  // TimerOne: library used to configure registers for ISR
  Timer1.initialize(ts);
  Timer1.attachInterrupt(ISR5kHz);
}

void loop(void){}