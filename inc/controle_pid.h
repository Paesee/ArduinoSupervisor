#ifndef CONTROLE_PID // diretiva de inclusão condicional para evitar múltiplas inclusões do mesmo arquivo de cabeçalho
#define CONTROLE_PID

// definicao da estrutura para o controlador PID
typedef struct ControladorPID
{
  // variavel para definicao do setpoint
  float tensao_referencia;
  // definicao dos "ganhos do numerador"
  float ganho_num_1;
  float ganho_num_2;
  float ganho_num_3;
  // definicao dos "ganhos do denominador"
  float ganho_den_1;
  float ganho_den_2;
  float ganho_den_3;
  // variaveis para armazenamento dos valores passados
  float ultimo_erro;
  float penultimo_erro;
  float antepenultimo_erro;
  float ultima_acao;
  float penultima_acao;
} ControladorPID_t;

// definicao das funcoes necessarias para execucao do controle
// funcao para inicializar a estrutura do controlador
void inicializaControladorPID(ControladorPID_t *pid, float gn1, float gn2, float gn3, float gd1, float gd2, float gd3);
// funcao para definir a tensao de referencia
void setTensaoReferencia(ControladorPID_t *pid, float tensao);
// funcao responsavel por executar a rotina de controle
float executaControladorPID(ControladorPID_t *pid, float tensao_medida);

#endif // fecha a diretiva de inclusão condicional