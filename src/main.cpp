// Include configuration file
#include <Config.h>

void setup(void)
{
  // PID Setup
  // P13 is used only for debugging ISR
  pinMode(13, OUTPUT);
  // P5 is used to output PWM using Timer0 on Phase-Correct Mode
  pinMode(5, OUTPUT);
  Timer1.initialize(TS);
  Timer1.attachInterrupt(ISR5kHz);
  // ConfigADC: configure to read A5
  configADC(A5);
  // PWM: setup registers to generate PWM signal on PD5
  configPWM();

  // ModbusRTU setup
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
}

void loop(void)
{
  // Checa por requisicoes Modbus RTU requests
  ModbusRTUServer.poll();
  // le o estado das entradas mapeadas e atualiza no servidor
  // entradas digitais
  for (int i = 0; i < NUMBER_DIGITAL_INPUTS; i++)
  {
    inputDigitalStatus[i] = digitalRead(inputDigitalMap[i]);
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
    digitalWrite(outputCoilMap[i], outputCoilStatus[i]);
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
}