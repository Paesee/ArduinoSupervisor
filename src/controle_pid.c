#include "controle_pid.h"

// implementacao das funcoes definadas no cabeçalho

// funcao para inicializar a estrutura do controlador
void inicializaControladorPID(ControladorPID_t *pid, float gn1, float gn2, float gn3, float gd1, float gd2, float gd3)
{
  pid->tensao_referencia = 0;
  pid->ganho_num_1 = gn1;
  pid->ganho_num_2 = gn2;
  pid->ganho_num_3 = gn3;
  pid->ganho_den_1 = gd1;
  pid->ganho_den_2 = gd2;
  pid->ganho_den_3 = gd3;
  pid->ultimo_erro = 0;
  pid->penultimo_erro = 0;
  pid->antepenultimo_erro = 0;
  pid->ultima_acao = 0;
  pid->penultima_acao = 0;
}

// funcao para definir a tensao de referencia
void setTensaoReferencia(ControladorPID_t *pid, float tensao){
  pid->tensao_referencia = tensao;
}

// funcao responsavel por executar a rotina de controle
float executaControladorPID(ControladorPID_t *pid, float tensao_medida){
  // calcula o erro
  float erro = pid->tensao_referencia - tensao_medida;
  // calcula a acao de controle
  float acao = pid->ganho_num_1 * erro + pid->ganho_num_2 * pid->ultimo_erro + pid->ganho_num_3 * pid->antepenultimo_erro - pid->ganho_den_3 * pid->penultima_acao;
  //atualiza os valores antigos para o erro
  pid->antepenultimo_erro = pid->penultimo_erro;
  pid->penultimo_erro = pid->ultimo_erro;
  pid->ultimo_erro = erro;
  //atualiza os valores antigos para a acao
  pid->penultima_acao = pid->ultima_acao;
  pid->ultima_acao = acao;
  // retorna o valor calculado para a acao de controle
  return acao;
}