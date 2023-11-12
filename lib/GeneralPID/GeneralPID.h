#ifndef GENERAL_PID_H // diretiva de inclusão condicional para evitar múltiplas inclusões do mesmo arquivo de cabeçalho
#define GENERAL_PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct GeneralPID
{
  //allows self reference
  struct GeneralPID *self;
  //setpoint for control routine
  float setpoint;
  //gain definitions
  float gain_1;
  float gain_2;
  float gain_3;
  float gain_4;
  //variables used to store past erros and outputs
  float error_n1;
  float error_n2;
  float output_n1;
  float output_n2;
  //pointers to functions
  int (*setSetpoint)(struct GeneralPID *self, float setpoint);
  float (*execute)(struct GeneralPID *self, float meas_variable);
} GeneralPID_t;

int GeneralPID_setSetpoint(GeneralPID_t *self, float setpoint);
float GeneralPID_execute(GeneralPID_t *self, float measurement);
GeneralPID_t *GeneralPID_creator(float g1, float g2, float g3, float g4);
int GeneralPID_destructor(GeneralPID_t *self);

#ifdef __cplusplus
}
#endif

#endif // fecha a diretiva de inclusão condicional