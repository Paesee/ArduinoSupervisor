// Include configuration file
#include <ConfigPID.h>
#include <ConfigModbus.h>

void setup(void)
{
  // PID Setup
  // P5 is used to output PWM using Timer0 on Phase-Correct Mode
  pinMode(5, OUTPUT);
  // PWM: setup registers to generate PWM signal on PD5
  configPWM();
  // Call TimerOne library functions
  Timer1.initialize(TS);
  Timer1.attachInterrupt(ISR5kHz);
  // ModbusRTU setup
  ModbusSetup();
}

void loop(void)
{
  pid.setGains(&pid, kp, ki, kd);
  pid.setSetpoint(&pid, setpoint);
  ModbusRoutine();
}