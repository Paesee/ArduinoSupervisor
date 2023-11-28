#include "GeneralPID.h"

#ifdef __cplusplus
extern "C"
{
#endif

  GeneralPID_t *GeneralPID_creator(float g1, float g2, float g3, float g4)
  {
    GeneralPID_t *newGeneralPID = (GeneralPID_t *)malloc(sizeof(GeneralPID_t));
    newGeneralPID->self = newGeneralPID;
    newGeneralPID->gain_1 = g1;
    newGeneralPID->gain_2 = g2;
    newGeneralPID->gain_3 = g3;
    newGeneralPID->gain_4 = g4;
    newGeneralPID->setpoint = 0;
    newGeneralPID->error_n1 = 0;
    newGeneralPID->error_n2 = 0;
    newGeneralPID->output_n1 = 0;
    newGeneralPID->output_n2 = 0;
    newGeneralPID->setSetpoint = GeneralPID_setSetpoint;
    newGeneralPID->setBoundaries = GeneralPID_setBoundaries;
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

  int GeneralPID_setBoundaries(GeneralPID_t *self, float max, float min)
  {
    self->max_output = max;
    self->min_output = min;
    return 0;
  }

  float GeneralPID_execute(GeneralPID_t *self, float measurement)
  {
    float error = self->setpoint - measurement;
    float output = self->gain_1 * error + self->gain_2 * self->error_n1 + self->gain_3 * self->error_n2 - self->gain_4 * self->output_n2;
    if (output > 50000)
      output = 50000;
    if (output < 0)
      output = 0;
    self->error_n2 = self->error_n1;
    self->error_n1 = error;
    self->output_n2 = self->output_n1;
    self->output_n1 = output;
    return 0.002*output;
  }

#ifdef __cplusplus
}
#endif