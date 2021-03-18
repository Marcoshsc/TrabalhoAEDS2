#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lista.h"

char *cria_str(char *str){
	char *s = (char *) calloc(strlen(str) + 1, sizeof(char));
	strcpy(s, str);
	return s;
}

Lista *cria(char *nome, Lista *prox){
	Lista *nomes = (Lista *) malloc(sizeof(Lista));
	nomes->nome = nome;
	nomes->prox = prox;
	return nomes;
}

void libera(Lista *nomes){
	Lista *temp = NULL;
	while(nomes) {
		temp = nomes->prox;
		free(nomes->nome);
		free(nomes);
		nomes = temp;
	}
}

int conta(Lista *nomes){
	int qtd = 0;
	while(nomes) {
		qtd += 1;
		nomes = nomes->prox;
	}
	return qtd;
}

void inverte(Lista *nomes){
	int i;
	int qtd = conta(nomes);
	Lista *atual = nomes;
	Lista **temp = (Lista **) malloc(sizeof(Lista *) * (qtd + 1));
	temp[qtd] = NULL;
	for (i = qtd - 1; i >= 0; i--) {
		temp[i] = atual;
		atual = atual->prox;
	}
	for (i = 0; i < qtd; i++) {
		temp[i]->prox = temp[i + 1];
	}
}

void imprime(Lista *nomes){
	while(nomes) {
		printf("| %s ", nomes->nome);
		nomes = nomes->prox;
	}
	printf("\n\n");
}


