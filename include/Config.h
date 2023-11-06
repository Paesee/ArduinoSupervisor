#ifndef CONFIG_H
#define CONFIG_H

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
void ISR5kHz(void);
void configADC(int pin);
float readAnalogic(void);

#endif