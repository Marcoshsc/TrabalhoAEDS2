#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include "funcionarios.h"
#define SEM_FILHO -1

/* Cada nó da árvore binária em memória interna armazena a informação do indice primario,
o valor rrn/byteoffset no arquivo, o ponteiro para subárvore à esquerda e à direita. */
typedef struct noA {
    int info;
    int rrn;
    struct noA *esq;
    struct noA *dir;
} TNoA;

/* Cada nó da árvore binária externa armazena a informação do registro,
o valor rrn/byteoffset para subárvore à esquerda e à direita. */
typedef struct No{

    TFunc item;   //Registro dentro do nó
    long pEsq;    //Nó filho da esquerda
    long pDir;    //Nó filho da direita

}TNoAX;

/*cria uma árvore binária vazia inicializando o nó raiz com null, com nenhum elemento */
TNoA *inicializa_arvore_binaria();

/*função que verifica se uma árvore é vazia */
int  vazia_arvore_binaria(TNoA* t);

/*imprime a árvore binária */
void imprime_arvore_binaria(TNoA *nodo);

/*função de inserção recursiva de nós na árvore binária*/
void inserir_arvore_binaria(TNoA **t, int chave, int rrn);

/*cria a árvore binária utilizando o indice primario ordenado, neste caso aproveitamos para armazenar também o valor do rrn nos nós*/
void cria_arvore_binaria_indice_primario(FILE *arqIndicePrimario, int nFunc);

/*função que verifica se um elemento pertence ou não à árvore */
int busca_arvore_binaria(TNoA* t, int chave);

/*cria somente um nó solto*/
TNoA *criaNo_arvore_binaria(int chave, int rrn);

/*faz uma inserção solta na raiz*/
void inserir_arvore_binaria_RAIZ(TNoA *t, int chave, int rrn);

/*imprime as chaves inseridas na árvore binária externa*/
void imprime_chaves_arvore_binaria_externa(FILE* arq_saida);

/*funções utilizadas para inserir na árvore de forma recursiva os elementos*/
void inserir_recursivo_arvore_binaria_externa(long int localraiz, TFunc p, FILE* arq);
void inserir_item_arvore_binaria_externa(TFunc p, FILE* arq);

/*cria a árvore binária externa, fazendo a leitura do arquivo de entrada e gravando no arquivo de saída*/
void criar_arvore_binaria_externa(FILE *arq_entrada, FILE*arq_saida);

/*funções que permitem buscar uma chave na árvore binária externa*/
void buscar_recursivo_arvore_binaria_externa(long localraiz, int chave, TFunc** resposta, FILE* arq);
TFunc* pesquisar_arvore_binaria_externa(int chave, FILE* arq);

#endif
