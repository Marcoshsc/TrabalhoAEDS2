#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

#include <stdio.h>

#define TAM_NOME 200

/*estrutura do registro funcionário com campos de tamanho fixo*/
typedef struct Funcionario {
    int cod;
    char nome[TAM_NOME];
    char cpf[15];
    char data_nascimento[11];
    double salario;
}TFunc;

/* cria um funcionário inicializando-o com os valores passados por parâmetro*/
TFunc *funcionario(int cod, char *nome, char *cpf, char *data, double salario);

/* o tamanho do registro, retornado pela função tamanho_registro() é dado em bytes (aula 03 arquivos)*/
int tamanho_registro();

/* Salva no arquivo out o funciónário na posição atual do cursor*/
void salva_funcionario(TFunc *func, FILE *out);

/* Lê um funcionário do arquivo in na posição atual do cursor, retorna um ponteiro para funcionário lido do arquivo*/
TFunc *le_funcionario(FILE *in);

/* imprime na tela as informações do funcionário */
void imprime_funcionario(TFunc *funcionario);

/*cria uma base de dados de funcionários solicitando dados ao usuário*/
void cria_base_dados_funcionarios(FILE* arq, FILE* arqIndicePrimario, FILE* arqIndicePrimarioNaoOrdenado, int nFunc);

/*cria uma base de dados TESTE de funcionários*/
void cria_base_dados_funcionarios_teste(FILE* arq, FILE* arqIndicePrimario,  FILE* arqIndicePrimarioNaoOrdenado, int nFunc);

/*imprime uma base de dados de funcionários lidos de um arquivo */
void imprime_base_dados_funcionarios(FILE *arq, int nFunc);

/*imprime uma arquivo de indice primario de funcionários*/
void imprime_indice_primario_funcionarios(FILE *arqIndicePrimario, int nFunc);

#endif
