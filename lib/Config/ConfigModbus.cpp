#include "ConfigModbus.h"

// Section for ModbusRTU
// armazena o valor dos holding registers
float setpoint = 0;
float kp = 0;
float ki = 0;
float kd = 0;
int duty_cycle = 0;
int emergency = 0;
// mapeamento
const int inputDigitalMap[] = {10, 9, 8};
const int inputAnalogMap[] = {A4, A5};
const int outputCoilMap[] = {6, 3, 4, emergency};
const int outputPwmMap[] = {};
// armazena o estado atual das entradas e saidas
uint8_t inputDigitalStatus[NUMBER_DIGITAL_INPUTS];
uint16_t inputAnalogStatus[NUMBER_ANALOG_INPUTS];
int outputCoilStatus[NUMBER_COILS];
int outputPwmStatus[NUMBER_PWM_OUTPUTS];
int holdingRegistersStatus[NUMBER_HOLDING_REGISTERS]; // registradores internos

void ModbusSetup(void)
{
  // Inicializa o servidor Modbus RTU com o endereco de escravo SLAVE_ID
  if (!ModbusRTUServer.begin(SLAVE_ID, BAUDRATE, UART_CONFIG))
  {
    while (1);
  }
  // configura entradas e saídas e seta o estado inicial delas
  for (int i = 0; i < NUMBER_DIGITAL_INPUTS; i++)
  {
    pinMode(inputDigitalMap[i], INPUT_PULLUP);
  }
  for (int i = 0; i < NUMBER_COILS; i++)
  {
    pinMode(outputCoilMap[i], OUTPUT);
    outputCoilStatus[i] = 0;
    digitalWrite(outputCoilMap[i], outputCoilStatus[i]);
  }
  for (int i = 0; i < NUMBER_PWM_OUTPUTS; i++)
  {
    outputPwmStatus[i] = 0;
    analogWrite(outputPwmMap[i], outputPwmStatus[i]);
  }
  for (int i = 0; i < NUMBER_HOLDING_REGISTERS; i++)
  {
    holdingRegistersStatus[i] = 0;
  }
  // associa registradores do servidor modbus para as entradas e saídas definidas
  ModbusRTUServer.configureDiscreteInputs(DIGITAL_INPUT_ADDRESS, NUMBER_DIGITAL_INPUTS);                         // configura entradas digitais a partir do endereço DIGITAL_INPUT_ADDRESS
  ModbusRTUServer.configureInputRegisters(ANALOG_INPUT_ADDRESS, NUMBER_ANALOG_INPUTS);                           // configura entradas analogicas a partir do endereço ANALOG_INPUT_ADDRESS
  ModbusRTUServer.configureCoils(COIL_ADDRESS, NUMBER_COILS);                                                    // configura bobinas a partir do endereço COIL_ADDRESS
  ModbusRTUServer.configureHoldingRegisters(PWM_OUTPUTS_ADDRESS, NUMBER_PWM_OUTPUTS + NUMBER_HOLDING_REGISTERS); // configura saidas analogicas a partir do endereço NUMBER_PWM_OUTPUTS
  // coloca os valores iniciais de Kp, Ki e setpoint iniciais no servidor nos holding registers
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 0, 0);                    // de 0 a 500, variavel interna 0, end 0x32 = 50
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 1, 0);                    // de 0 a 100, variavel interna 1, end 0x33 = 51
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 2, 0);                    // de 0 a 100, variavel interna 2, end 0x34 = 52
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 3, 0);                    // de 0 a 100, variavel interna 3, end 0x35 = 53
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 4, pwm_ctrl_action);                // 0 a 500, variavel interna 5, end 0x37 = 55
}

void ModbusRoutine()
{
  // Checa por requisicoes Modbus RTU requests
  ModbusRTUServer.poll();
  // le o estado das entradas mapeadas e atualiza no servidor
  // entradas digitais
  for (int i = 0; i < NUMBER_DIGITAL_INPUTS; i++)
  {
    inputDigitalStatus[i] = !digitalRead(inputDigitalMap[i]);
  }
  ModbusRTUServer.writeDiscreteInputs(DIGITAL_INPUT_ADDRESS, inputDigitalStatus, NUMBER_DIGITAL_INPUTS);
  // entradas analogicas
  for (int i = 0; i < NUMBER_ANALOG_INPUTS; i++)
  {
    inputAnalogStatus[i] = analogRead(inputAnalogMap[i]);
  }
  ModbusRTUServer.writeInputRegisters(ANALOG_INPUT_ADDRESS, inputAnalogStatus, NUMBER_ANALOG_INPUTS);
  // saidas digitais
  for (int i = 0; i < NUMBER_COILS; i++)
  {
    outputCoilStatus[i] = ModbusRTUServer.coilRead(COIL_ADDRESS + i); // le o estado atual da saida no servidor
    if(i<3)
    {
      digitalWrite(outputCoilMap[i], outputCoilStatus[i]);
    }else
    {
      emergency = outputCoilStatus[3];
    }
  }
  // saidas PWM
  for (int i = 0; i < NUMBER_PWM_OUTPUTS; i++)
  {
    outputPwmStatus[i] = ModbusRTUServer.holdingRegisterRead(PWM_OUTPUTS_ADDRESS + i);
    analogWrite(outputPwmMap[i], outputPwmStatus[i]);
  }
  // registradores internos
    for (int i = 0; i < NUMBER_HOLDING_REGISTERS; i++)
  {
    holdingRegistersStatus[i] = ModbusRTUServer.holdingRegisterRead(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + i);
  }
  //lê
  setpoint = 0.001 * ModbusRTUServer.holdingRegisterRead(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 0);  // setpoint registrador offset 0
  kp = 1 * ModbusRTUServer.holdingRegisterRead(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 1);  // setpoint registrador offset 1
  ki = 1 * ModbusRTUServer.holdingRegisterRead(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 2);  // setpoint registrador offset 2
  kd = 1 * ModbusRTUServer.holdingRegisterRead(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 3);  // setpoint registrador offset 3
  //escreve
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 0, setpoint * 1000);                    // de 0 a 500, variavel interna 0, end 0x32 = 50
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 1, kp * 1);                    // de 0 a 100, variavel interna 1, end 0x33 = 51
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 2, ki * 1);                    // de 0 a 100, variavel interna 2, end 0x34 = 52
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 3, kd * 1);                    // de 0 a 100, variavel interna 3, end 0x35 = 53
  ModbusRTUServer.holdingRegisterWrite(PWM_OUTPUTS_ADDRESS + NUMBER_PWM_OUTPUTS + 4, pwm_ctrl_action);                // 0 a 500, variavel interna 4, end 0x36 = 54
}