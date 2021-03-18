#ifndef LISTA_H
#define LISTA_H

typedef struct Lista {
	char *nome;
	struct Lista *prox;
} Lista;

/*imprime lista de nomes*/
void imprime(Lista *nomes);

/*aloca copia de str*/
char *cria_str(char *str);

/*cria lista encadeada de nomes. Lembrar de chamar libera_nomes(nomes)*/
Lista *cria(char *nome, Lista *prox);

/*desaloca lista encadeada de nomes*/
void libera(Lista *nomes);

/*retorna quantidade de nomes*/
int conta(Lista *nomes);

/*inverte lista encadeada de nomes*/
void inverte(Lista *nomes);

#endif
