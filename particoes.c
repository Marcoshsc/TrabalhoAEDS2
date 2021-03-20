
#include "funcionarios.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "particoes.h"

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(arq); //posiciona cursor no inicio do arquivo


    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

/*Algoritmo a ser implementado no trabalho de geracao de particoes por Selecao Natural recebe como parâmetro
o arquivo de dados de entrada, a lista contendo os nomes dos arquivos de saída das partições,
o númer de elementos M a ser armazenado em cada partição e o número de registro total do arquivo e o tamanho do reservatório.
Fiquem a vontade para modificar a estrutura, foi uma sugestão.*/
void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n, int* numeroNomes){
    rewind(arq);
    Lista* nomes = nome_arquivos_saida;
    Lista* prev = NULL;
    FILE* repo = fopen("repository.dat", "w+");
    for (int tam = 0; tam < nFunc; tam += M)
    {
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (tam + i) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
            i++;
            if(i>=M) break;
        }

        if (i != M) {
            i = M;
        }

        while(i > 0) {

            char* newNome = malloc(5 * sizeof(char));
            if(*numeroNomes > 50)
                break;
            if(!nomes) {
                (*numeroNomes)++;
                sprintf(newNome, "p%d.dat", *numeroNomes);
                prev->prox = cria(newNome, NULL);
                nomes = prev->prox;
            }
            FILE* part = fopen(nomes->nome, "w+");
            rewind(part);

            int found[i];
            for (int k = 0; k < i; k++)
            {
                found[k] = 0;
            }
            
            int inPartition = 1;
            int inRepository = 0;
            int j = 1;
            int current = findSmaller(v, found, i);
            salva_funcionario(v[current], part);
            nomes->tamanho++;
            found[current] = 1;
            if(j == i) {
                fclose(part);
                if(!prev)
                    prev = nomes;
                else
                    prev = prev->prox;
                nomes = nomes->prox;
                break;
            }
            while(inRepository < n && j < i) {
                int smaller = findSmaller(v, found, i);
                found[smaller] = 1;
                j++;
                if(v[smaller]->cod < v[current]->cod) {
                    salva_funcionario(v[smaller], repo);
                    inRepository++;
                }
                else {
                    fseek(part, inPartition * tamanho_registro(), SEEK_SET);
                    salva_funcionario(v[smaller], part);
                    nomes->tamanho++;
                    inPartition++;
                    current = smaller;
                }
            }

            if(inRepository == 0) {
                if(!prev)
                    prev = nomes;
                else
                    prev = prev->prox;
                nomes = nomes->prox;
                break;
            }
            
            int newI = 0;
            TFunc* newV[M];
            for (int k = 0; k < i; k++)
            {
                if(!found[k]) {
                    newV[k] = v[k];
                    newI++;
                }
            }
            for (int k = 0; k < inRepository; k++)
            {
                fseek(repo, k * tamanho_registro(), SEEK_SET);
                TFunc* func = le_funcionario(repo);
                newV[newI] = func;
                newI++;
            }

            rewind(repo);
            inRepository = 0;
            i = newI;
            for (int k = 0; k < newI; k++)
            {
                v[k] = newV[k];
            }

            fclose(part);
            if(!prev)
                prev = nomes;
            else
                prev = prev->prox;
            nomes = nomes->prox;
        }
    }
    
}

int findSmaller(TFunc* v[], int found[], int tam) {
    int smaller = -1;
    for (int k = 0; k < tam; k++)
    {
        if(smaller == -1 && !found[k]) {
            smaller = k;
            continue;
        }
        if(!found[k] && smaller > v[k]->cod)
            smaller = k;
    }
    return smaller;
}
