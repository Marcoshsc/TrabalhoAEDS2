#include "intercalacao.h"
#include "funcionarios.h"
#include "arvore_binaria.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes)
{

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out;   //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL)
    {
        printf("Erro ao abrir arquivo de sa?da\n");
    }
    else
    {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i = 0; i < num_p; i++)
        {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL)
            {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[i].f);
                if (f == NULL)
                {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "", "", "", 0);
                }
                else
                {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else
            {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim))
        { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for (int i = 0; i < num_p; i++)
            {
                if (v[i].func->cod < menor)
                {
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX)
            {
                fim = 1; //terminou processamento
            }
            else
            {
                //salva funcionario no arquivo de saída
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[pos_menor].func, out);
                //atualiza posição pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[pos_menor].f);
                aux++;
                if (f == NULL)
                {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiçao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "", 0.0);
                }
                else
                {
                    v[pos_menor].func = f;
                }
            }
        }

        //fecha arquivos das partiÇões de entrada
        for (int i = 0; i < num_p; i++)
        {
            fclose(v[i].f);
            //    free(v[i].func);
        }
        //fecha arquivo de saída
        fclose(out);
    }
}

void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p)
{
    int baseSize = num_p;
    int totalLevels = 0;
    for (int i = baseSize; i > 0; i /= 2)
        totalLevels++;

    TNoA *base[baseSize];
    // cria a base da arvore
    for (int i = 0; i < num_p; i++)
    {
        TFunc *func = pop(pilha[i], 0, &vetTop[i]);
        TNoA *no = criaNo_arvore_binaria(func->cod, -1);
        base[i] = no;
    }
    imprime_arvore_binaria(base[0]);
    TNoA *paiPrincipal;
    // TNoA *pais[(int)ceil(baseSize / 2.0)];
    // TNoA *novosPais[(int)ceil(baseSize / 2.0)];
    TNoA *pais[baseSize];
    TNoA *novosPais[baseSize];
    int iteration = 0;
    int totalElements = 0;
    int totalElementsNovosPais = 0;
    while (1)
    {
        TNoA **toUse = iteration == 0 ? base : pais;
        int sizeToBeUsed = iteration == 0 ? baseSize : totalElements;
        printf("Iteration: %d\n", iteration);
        printf("Tamanho para ser usado: %d\n", sizeToBeUsed);
        printf("Começou um for\n");
        for (int i = 0; i < sizeToBeUsed; i += 2)
        {
            printf("%d\n", i);
            if (i + 1 == sizeToBeUsed)
            {
                printf("Entrou no if\n");
                TNoA *no = criaNo_arvore_binaria(toUse[i]->info, -1);
                no->esq = toUse[i];
                no->dir = NULL;
                if (iteration == 0)
                {
                    pais[totalElements] = no;
                    totalElements++;
                }
                else
                {
                    novosPais[totalElementsNovosPais] = no;
                    totalElementsNovosPais++;
                }
                printf("Terminou o if\n");
                continue;
            }

            if (toUse[i]->info < toUse[i + 1]->info)
            {
                TNoA *no = criaNo_arvore_binaria(toUse[i]->info, -1);
                no->esq = toUse[i];
                no->dir = toUse[i + 1];
                if (iteration == 0)
                {
                    pais[totalElements] = no;
                    totalElements++;
                }
                else
                {
                    novosPais[totalElementsNovosPais] = no;
                    totalElementsNovosPais++;
                }
            }
            else
            {
                TNoA *no = criaNo_arvore_binaria(toUse[i + 1]->info, -1);
                no->esq = toUse[i];
                no->dir = toUse[i + 1];
                if (iteration == 0)
                {
                    pais[totalElements] = no;
                    totalElements++;
                }
                else
                {
                    novosPais[totalElementsNovosPais] = no;
                    totalElementsNovosPais++;
                }
            }
        }

        int toCompare = iteration == 0 ? totalElements : totalElementsNovosPais;
        printf("Numero de pais gerados: %d\n", toCompare);
        if (toCompare == 1)
        {
            paiPrincipal = iteration == 0 ? pais[0] : novosPais[0];
            imprime_arvore_binaria(paiPrincipal);
            printf("Saiu\n");
            break;
        }
        if (iteration == 0)
        {
            iteration++;
            continue;
        }
        for (int j = 0; j < totalElementsNovosPais; j++)
        {
            pais[j] = novosPais[j];
        }
        totalElements = totalElementsNovosPais;
        totalElementsNovosPais = 0;
        iteration++;
    }

    printf("\n Saiu while %p\n", paiPrincipal);
    imprime_arvore_binaria(paiPrincipal);
}
