#ifndef PILHA_H
#define PILHA_H

#include "funcionarios.h"

typedef struct pilha {
    TFunc *info;
    int p;
} TPilha;

/*inicializa a pilha*/
void inicializa(int *base, int *lim, int *topo);

/*verifica se a pilha esta cheia a vazia*/
int pilha_cheia(int lim, int topo);
int pilha_vazia(int base, int topo);

/*Insere e remove elemento do topo da pilha, Insere info no topo da pilha, retorna -1 caso não consiga inserir ou remover na pilha, caso contrário retorna o funcionário*/
TFunc *push(TPilha *pilha, int lim, int *topo, TFunc *info, int p);
TFunc *pop(TPilha *pilha, int base, int *topo);

/*consulta e retorna um elemento funcionario da pilha e o id "p" referente à partição, retorna -1 caso pilha esteja vazia*/
TFunc *peek_func(TPilha *pilha, int base, int *topo);
int peek_fp(TPilha *pilha, int base, int *topo);

/*imprime a pilha*/
void imprime_pilha(TPilha *pilha, int *topo);

/*cria uma pilha de nFunc funcionários lidos de um arquivo*/
void cria_pilha_particao(TPilha *pilha, FILE *arq, int nFunc, int *vetTopo);

#endif
