#include "GeneralPID.h"

GeneralPID_t *GeneralPID_creator(float gn1, float gn2, float gn3, float gd1, float gd2, float gd3)
{
  GeneralPID_t *newGeneralPID = (GeneralPID_t *)malloc(sizeof(GeneralPID_t));
  newGeneralPID->self = newGeneralPID;
  newGeneralPID->num_gain_1 = gn1;
  newGeneralPID->num_gain_2 = gn2;
  newGeneralPID->num_gain_3 = gn3;
  newGeneralPID->den_gain_1 = gd1;
  newGeneralPID->den_gain_2 = gd2;
  newGeneralPID->den_gain_3 = gd3;
  newGeneralPID->setpoint = 0;
  newGeneralPID->error_n1 = 0;
  newGeneralPID->error_n2 = 0;
  newGeneralPID->output_n1 = 0;
  newGeneralPID->output_n2 = 0;
  newGeneralPID->setSetpoint = GeneralPID_setSetpoint;
  newGeneralPID->execute = GeneralPID_execute;
  return newGeneralPID;
}

int GeneralPID_destructor(GeneralPID_t *self)
{
  free(self);
  return 0;
}

int GeneralPID_setSetpoint(GeneralPID_t *self, float setpoint)
{
  self->setpoint = setpoint;
  return 0;
}

float GeneralPID_execute(GeneralPID_t *self, float measurement)
{
  float error = self->setpoint - measurement;
  float output = self->num_gain_1 * error + self->num_gain_2 * self->error_n1 + self->num_gain_3 * self->error_n2 - self->den_gain_3 * self->output_n2;
  self->error_n2 = self->error_n1;
  self->error_n1 = error;
  self->output_n2 = self->output_n1;
  self->output_n1 = output;
  return output;
}