#include "Config.h"

// Section for PID Control
// Constants definition
const float ANALOG2VOLTAGE = 0.00489;
const int OFFSET = -10;
const int FS = 5000;
const float TS = 1e6 * (1.0 / FS);
// Variables definition for analog read
int meas = 0;
bool is_converted = true;
float voltage = 0;
// Variables definition for PID Voltage Control
GeneralPID_t *PID = GeneralPID_creator(10553, -19995, 9453, -1);
float ctrl_action = 0;
int pwm_ctrl_action = 0;

void ISR5kHz(void)
{
  // digitalWrite(13, HIGH);
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
  ctrl_action = PID->execute(PID->self, voltage);
  OCR0B = pwm_ctrl_action;
  // digitalWrite(13, LOW);
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
  OCR0B = 50;
}

// Section for ModbusRTU
const int inputDigitalMap[] = {22, 23, 24, 25, 26, 27, 28, 29};
const int inputAnalogMap[] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int outputCoilMap[] = {30, 31, 32, 33, 34, 35, 36, 37};
const int outputPwmMap[] = {2, 3, 4, 5, 6, 7, 8, 9};
// armazena o estado atual das entradas e saidas
uint8_t inputDigitalStatus[NUMBER_DIGITAL_INPUTS];
uint16_t inputAnalogStatus[NUMBER_ANALOG_INPUTS];
int outputCoilStatus[NUMBER_COILS];
int outputPwmStatus[NUMBER_PWM_OUTPUTS];
int holdingRegistersStatus[NUMBER_HOLDING_REGISTERS]; // registradores internos