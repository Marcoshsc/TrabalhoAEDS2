
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

// /*Algoritmo a ser implementado no trabalho de geracao de particoes por Selecao Natural recebe como parâmetro
// o arquivo de dados de entrada, a lista contendo os nomes dos arquivos de saída das partições,
// o númer de elementos M a ser armazenado em cada partição e o número de registro total do arquivo e o tamanho do reservatório.
// Fiquem a vontade para modificar a estrutura, foi uma sugestão.*/
// void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n, int* numeroNomes){
//     rewind(arq);
//     Lista* nomes = nome_arquivos_saida;
//     Lista* prev = NULL;
//     FILE* repo = fopen("repository.dat", "w+");
//     for (int tam = 0; tam < nFunc; tam += M)
//     {
//         TFunc *v[M];
//         int i = 0;
//         while (!feof(arq)) {
//             fseek(arq, (tam + i) * tamanho_registro(), SEEK_SET);
//             v[i] = le_funcionario(arq);
//             i++;
//             if(i>=M) break;
//         }

//         if (i != M) {
//             i = M;
//         }

//         while(i > 0) {

//             if(!nomes) {
//                 char* newNome = malloc(5 * sizeof(char));
//                 (*numeroNomes)++;
//                 sprintf(newNome, "p%d.dat", *numeroNomes);
//                 prev->prox = cria(newNome, NULL);
//                 nomes = prev->prox;
//             }
//             FILE* part = fopen(nomes->nome, "w+");
//             rewind(part);

//             int found[i];
//             for (int k = 0; k < i; k++)
//             {
//                 found[k] = 0;
//             }
            
//             int inPartition = 1;
//             int inRepository = 0;
//             int j = 1;
//             int current = findSmaller(v, found, i);
//             salva_funcionario(v[current], part);
//             nomes->tamanho++;
//             found[current] = 1;
//             if(j == i) {
//                 fclose(part);
//                 if(!prev)
//                     prev = nomes;
//                 else
//                     prev = prev->prox;
//                 nomes = nomes->prox;
//                 break;
//             }
//             while(inRepository < n && j < i) {
//                 int smaller = findSmaller(v, found, i);
//                 found[smaller] = 1;
//                 j++;
//                 if(v[smaller]->cod < v[current]->cod) {
//                     salva_funcionario(v[smaller], repo);
//                     inRepository++;
//                 }
//                 else {
//                     fseek(part, inPartition * tamanho_registro(), SEEK_SET);
//                     salva_funcionario(v[smaller], part);
//                     nomes->tamanho++;
//                     inPartition++;
//                     current = smaller;
//                 }
//             }

//             if(inRepository == 0) {
//                 if(!prev)
//                     prev = nomes;
//                 else
//                     prev = prev->prox;
//                 nomes = nomes->prox;
//                 break;
//             }
            
//             int newI = 0;
//             TFunc* newV[M];
//             for (int k = 0; k < i; k++)
//             {
//                 if(!found[k]) {
//                     newV[k] = v[k];
//                     newI++;
//                 }
//             }
//             for (int k = 0; k < inRepository; k++)
//             {
//                 fseek(repo, k * tamanho_registro(), SEEK_SET);
//                 TFunc* func = le_funcionario(repo);
//                 newV[newI] = func;
//                 newI++;
//             }

//             rewind(repo);
//             inRepository = 0;
//             i = newI;
//             for (int k = 0; k < newI; k++)
//             {
//                 v[k] = newV[k];
//             }

//             fclose(part);
//             if(!prev)
//                 prev = nomes;
//             else
//                 prev = prev->prox;
//             nomes = nomes->prox;
//         }
//     }
    
// }

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n, int* numeroNomes) {
    int totalLidos = 0;
    Lista* nomes = nome_arquivos_saida;
    TFunc** v = malloc(M * sizeof(TFunc*));
    int inVet = 0;

    FILE* repo = fopen("repository.dat", "w+");
    int inRepo = 0;

    Lista* nomeAtual = nomes;
    FILE* currentPart = NULL;
    int inCurrentPart = 0;

    while(canReadMore(totalLidos, nFunc)) {
        if(inVet < M) {
            v[inVet] = getFuncionario(arq, &totalLidos);
            inVet++;
        }

        if(inVet < M)
            continue;
        
        int smaller = findSmaller(v, inVet);
        if(currentPart == NULL) {
            char* nome = nomes->nome;
            currentPart = openPartition(nome);
            nomeAtual = nomes;

            updatePartitionNames(nomes, numeroNomes);
            nomes = nomes->prox;
        }
        if(inCurrentPart >= M) {
            fclose(currentPart);
            currentPart = NULL;
            inCurrentPart = 0;
            continue;
        }
        fseek(currentPart, inCurrentPart * tamanho_registro(), SEEK_SET);
        salva_funcionario(v[smaller], currentPart);
        nomeAtual->tamanho++;
        inCurrentPart++;

        if(!canReadMore(totalLidos, nFunc)) {
            for (int i = smaller; i < inVet - 1; i++) {
                v[i] = v[i + 1];
            }
            inVet--;
            break;
        }

        int code = v[smaller]->cod;
        free(v[smaller]);
        v[smaller] = getFuncionario(arq, &totalLidos);
        
        smaller = findSmaller(v, inVet);

        if(v[smaller]->cod < code) {
            fseek(repo, inRepo * tamanho_registro(), SEEK_SET);
            salva_funcionario(v[smaller], repo);
            inRepo++;
            if(!canReadMore(totalLidos, nFunc)) {
                for (int i = smaller; i < inVet - 1; i++) {
                    v[i] = v[i + 1];
                }
                inVet--;
                break;
            }
            v[smaller] = getFuncionario(arq, &totalLidos);
        }

        if(inRepo < n) {
            continue;
        }
        fclose(currentPart);

        char* nome = nomes->nome;
        currentPart = openPartition(nome);
        nomeAtual = nomes;
        updatePartitionNames(nomes, numeroNomes);
        nomes = nomes->prox;

        int* lastSaved = NULL;
        int i = 0;
        while(inVet > 0)
        {
            fseek(currentPart, i * tamanho_registro(), SEEK_SET);
            int lower = findSmaller(v, inVet);
            salva_funcionario(v[lower], currentPart);
            nomeAtual->tamanho++;
            for (int i = lower; i < inVet - 1; i++) {
                v[i] = v[i + 1];
            }
            inVet--;
            i++;
        }

        for (int i = 0; i < inVet; i++)
            free(v[i]);
        inVet = 0;
        for (int i = 0; i < inRepo; i++)
        {
            fseek(repo, i * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(repo);
            inVet++;
        }
        inRepo = 0;

        fclose(currentPart);
        currentPart = NULL;
        inCurrentPart = 0;


    }

    if(currentPart) {
        fclose(currentPart);
        currentPart = NULL;
        inCurrentPart = 0;
    }
    int inLastPartition = 0;
    if(!(inVet <= 0 && inRepo <= 0)) {

        if(inRepo > 0) {
            char* nome = nomes->nome;
            currentPart = openPartition(nome);
            nomeAtual = nomes;
            inCurrentPart = 0;
            for (int i = 0; i < inRepo; i++)
            {
                fseek(repo, i * tamanho_registro(), SEEK_SET);
                TFunc* func = le_funcionario(repo);

                fseek(currentPart, inCurrentPart * tamanho_registro(), SEEK_SET);
                salva_funcionario(func, currentPart);
                nomeAtual->tamanho++;

                inCurrentPart++;
            }
            fclose(currentPart);
            inCurrentPart = 0;
        }
        if(inVet > 0) {
            updatePartitionNames(nomes, numeroNomes);
            nomes = nomes->prox;
            char* nome = nomes->nome;
            currentPart = openPartition(nome);
            nomeAtual = nomes;
            inCurrentPart = 0;
            for (int i = 0; i < inVet; i++)
            {
                fseek(currentPart, i * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], currentPart);
                nomeAtual->tamanho++;
                inCurrentPart++;
            }
            fclose(currentPart);
        }
    }
    fclose(repo);
    free(v);
}

TFunc* getFuncionario(FILE* arq, int* totalLidos) {
    fseek(arq, *totalLidos * tamanho_registro(), SEEK_SET);
    TFunc* func = le_funcionario(arq);
    (*totalLidos)++;
    return func;
}

void updatePartitionNames(Lista* nomes, int* numeroNomes) {
    if(nomes->prox == NULL) {
        char* newNome = malloc(5 * sizeof(char));
        (*numeroNomes)++;
        sprintf(newNome, "p%d.dat", *numeroNomes);
        nomes->prox = cria(newNome, NULL);
    }
}

int canReadMore(int totalLidos, int nFunc) {
    return totalLidos < nFunc;
}

FILE* openPartition(char* nome) {
    FILE* file = fopen(nome, "w+");
    return file;
}

int findSmaller(TFunc* v[], int tam) {
    int smaller = 0;
    for (int k = 1; k < tam; k++)
    {
        if(v[smaller]->cod > v[k]->cod)
            smaller = k;
    }
    return smaller;
}
