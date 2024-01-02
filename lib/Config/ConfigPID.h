#ifndef CONFIGPID_H
#define CONFIGPID_H

// Libraries to be included
#include <Arduino.h>
#include "GeneralPID.h"
#include "TimerOne.h"
#include "ConfigModbus.h"

// Constants definition
extern const int FS;
extern const float TS;
extern const float ANALOG2VOLTAGE;
extern const int OFFSET;
// Variables definition for analog read
extern int meas;
extern bool is_converted;
extern float voltage;
// Variables definition for PID Voltage Control
extern GeneralPID pid;
extern float ctrl_action;
extern float pwm_factor;
extern int pwm_ctrl_action;
// Function prototypes
void ISR5kHz(void);
void configADC(int pin);
void configPWM(void);

#endif