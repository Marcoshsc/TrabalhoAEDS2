
#include <stdio.h>
#include <stdlib.h>
#include "funcionarios.h"
#include "lista.h"
#define SEM_FILHO -1
#define TRUE "TRUE"
#define FALSE "FALSE"

typedef struct noB
{
    char separador[15];
    int rrn;
    char leaf[5];
    int bloco;
    struct noB *pai;
    struct noB *esq;
    struct noB *dir;

} TNoB;

typedef struct Tnos
{
    char *separador;
    struct Tnos *prox;

} TNos;

TNoB *criaNo_arvore_B(char *separador);
void freeNoB(TNoB *no);
void inserir_arvore_B_RAIZ(TNoB *t, char separador[], int rrn);
TNos *criaNos(char *nome, TNos *prox);
void FindSeparator(char *key1, char *key2, char *sep);
void ordemAlfabetica(FILE *arq, int nFunc, int *ordem[]);
Lista *BlocosEncadeados(FILE *arq, int nFunc);
const char *Separator(char key1[], char key2[]);
void criaBMais(char *nome_arquivo, int M, int nFunc);
void imprimeNoB(TNoB *no);
int isLeaf(TNoB *no);

void criaIndice(FILE *arq, TNoB **nos, int tam);
void imprimeIndice(char *nomeArq, int tam);