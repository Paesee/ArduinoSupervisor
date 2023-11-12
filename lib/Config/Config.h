#ifndef CONFIG_H
#define CONFIG_H

// Libraries to be included
#include <Arduino.h>
#include "GeneralPID.h"
#include "TimerOne.h"
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <stdlib.h>

// Section for PID Control
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
extern GeneralPID_t *PID;
extern float ctrl_action;
extern float pwm_factor;
extern int pwm_ctrl_action;
// Functions prototype
void ISR5kHz(void);
void configADC(int pin);
void configPWM(void);

// Section for ModbusRTU COM
// Variables definition
extern int i;
// definicoes do escravo e da comunicao serial
#define SLAVE_ID 10
#define BAUDRATE 9600
#define UART_CONFIG SERIAL_8N2
// numero de entradas e saidas
#define NUMBER_DIGITAL_INPUTS 8    // 8 entradas digitais
#define NUMBER_ANALOG_INPUTS 8     // 8 entradas analogicas
#define NUMBER_COILS 8             // 8 saidas digitais
#define NUMBER_PWM_OUTPUTS 8       // 8 saidas PWM
#define NUMBER_HOLDING_REGISTERS 8 // 8 registradores holding internos (a partir do endereco PWM_OUTPUTS_ADDRESS+NUMBER_PWM_OUTPUTS)
// enderecos iniciais dos registradores para tipo de entrada e saida
#define DIGITAL_INPUT_ADDRESS 0x0000
#define ANALOG_INPUT_ADDRESS 0x0010 // 16 enderecos para cada tipo
#define COIL_ADDRESS 0x0020
#define PWM_OUTPUTS_ADDRESS 0x0030
// mapeamento dos pinos de entradas e saidas
extern const int inputDigitalMap[];
extern const int inputAnalogMap[];
extern const int outputCoilMap[];
extern const int outputPwmMap[];
// armazena o estado atual das entradas e saidas
extern uint8_t inputDigitalStatus[NUMBER_DIGITAL_INPUTS];
extern uint16_t inputAnalogStatus[NUMBER_ANALOG_INPUTS];
extern int outputCoilStatus[NUMBER_COILS];
extern int outputPwmStatus[NUMBER_PWM_OUTPUTS];
extern int holdingRegistersStatus[NUMBER_HOLDING_REGISTERS]; // registradores internos

#endif
