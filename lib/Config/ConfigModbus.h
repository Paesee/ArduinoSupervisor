#ifndef CONFIGMODBUS_H
#define CONFIGMODBUS_H

// Libraries to be included
#include <Arduino.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <stdlib.h>
#include "ConfigPID.h"

// Constants definitions
// ModbusRTU config
#define SLAVE_ID 1
#define BAUDRATE 57600
#define UART_CONFIG SERIAL_8N2
// Inputs and outputs configuration
#define NUMBER_DIGITAL_INPUTS 3    // 3 entradas digitais
#define NUMBER_ANALOG_INPUTS 2     // 2 entradas analogicas
#define NUMBER_COILS 4             // 8 saidas digitais
#define NUMBER_PWM_OUTPUTS 0       // 0 saidas PWM
#define NUMBER_HOLDING_REGISTERS 5 // 5 registradores holding internos (a partir do endereco PWM_OUTPUTS_ADDRESS+NUMBER_PWM_OUTPUTS)
// Addresses of inputs and outputs definition 
#define DIGITAL_INPUT_ADDRESS 0x0000
#define ANALOG_INPUT_ADDRESS 0x0010 // 16 enderecos para cada tipo
#define COIL_ADDRESS 0x0020
#define PWM_OUTPUTS_ADDRESS 0x0030
// Variables definition
extern float setpoint;
extern float kp;
extern float ki;
extern float kd;
extern int duty_cycle;
extern int emergency;
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
// Function prototypes
void ModbusSetup(void);
void ModbusRoutine(void);

#endif
