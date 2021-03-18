#include "pilha.h"

int pilha_cheia(int lim, int topo) {
    if (topo == lim)
        return 1;
    else
        return 0;
}

int pilha_vazia(int base, int topo) {
    if (topo < base)
        return 1; //pilha vazia
    else
        return 0; //pilha tem pelo menos 1 elemento
}

TFunc *push(TPilha *pilha, int lim, int *topo, TFunc *info, int p){
    if (pilha_cheia(lim, *topo)) {
        return NULL;
    }
    else {
        //faz a inserção
        *topo = *topo + 1;
        pilha[*topo].info = info;
        pilha[*topo].p = p;
        return info;
    }
}

TFunc *pop(TPilha *pilha, int base, int *topo) {
    if (pilha_vazia(base, *topo)) {
        return NULL;
    }
    else {
        //faz a remoção
        TFunc *info = pilha[*topo].info;
        *topo = *topo - 1;
        return info;
    }
}

TFunc *peek_func(TPilha *pilha, int base, int *topo) {
    if (pilha_vazia(base, *topo))
        return NULL;
    else {
        //faz consulta
        return pilha[*topo].info;
    }
}

int peek_fp(TPilha *pilha, int base, int *topo) {
    if (pilha_vazia(base, *topo))
        return NULL;
    else {
        //faz consulta
        return pilha[*topo].p;
    }
}

void imprime_pilha(TPilha *pilha, int *topo) {
    printf("\nEstado atual da Pilha:\n");
    for (int i = *topo; i >= 0; i--) {
        printf("posição [%d]: %d\n", i, pilha[i].info->cod);
    }
    printf("\n");
}

void cria_pilha_particao(TPilha *pilha, FILE *arq, int nFunc, int *vetTopo){

    int lim=nFunc-1;

    rewind(arq);
    int i = nFunc-1;
    fseek(arq,  0 * sizeof (TFunc), SEEK_SET);
    if(arq != NULL) {
        int aux = 1;
        do{
            fseek(arq, (i) * tamanho_registro(), SEEK_SET);
            TFunc *func = le_funcionario(arq);
            if(func){
                push(pilha,lim,vetTopo,func,arq);
                i--;
            }else aux = 0;
        }while(aux);
    }
    else printf("Erro ao abrir arquivo\n");

    imprime_pilha(pilha,vetTopo);
}
