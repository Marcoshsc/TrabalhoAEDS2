#include "intercalacao.h"
#include "funcionarios.h"
#include "arvore_binaria.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes)
{

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out;   //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL)
    {
        printf("Erro ao abrir arquivo de saida\n");
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

void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p, int nFunc)
{
    int baseSize = num_p;

    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")))
    {
        printf("Intercalação com arvore de vencedores\n");
    }
    else
    {
        printf("Erro ao abrir arquivo de saida\n");
        return;
    }
    TNoA **base = malloc(baseSize * sizeof(TNoA*));
    TFunc **hash = malloc(nFunc * sizeof(TFunc*));
    // cria a base da arvore
    for (int i = 0; i < num_p; i++)
    {
        TFunc *func = pop(pilha[i], 0, &vetTop[i]);
        hash[func->cod] = func;
        TNoA *no = criaNo_arvore_binaria(func->cod, -1);
        no->pilha = i;
        base[i] = no;
        
    }
    
    
    TNoA *paiPrincipal;
    TNoA **pais = malloc(baseSize * sizeof(TNoA*));
    TNoA **novosPais = malloc(baseSize * sizeof(TNoA*));
    int iteration = 0;
    int totalElements = 0;
    int totalElementsNovosPais = 0;
    int funcNoArquivo = 0;

    // gera a primeira versao da arvore de vencedores
    while (1) //primeira iteração
    {
        TNoA **toUse = iteration == 0 ? base : pais;
        int sizeToBeUsed = iteration == 0 ? baseSize : totalElements;
        for (int i = 0; i < sizeToBeUsed; i += 2) //cria os nós na primeira iteraçã
        {
            if (i + 1 == sizeToBeUsed)
            {
                TNoA *no = criaNo_arvore_binaria(toUse[i]->info, -1);
                no->pilha = toUse[i]->pilha;
                no->esq = toUse[i];
                no->dir = NULL;
                no->esq->pai = no;
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
                continue;
            }

            if (toUse[i]->info < toUse[i + 1]->info)
            {
                TNoA *no = criaNo_arvore_binaria(toUse[i]->info, -1);
                no->pilha = toUse[i]->pilha;
                no->esq = toUse[i];
                no->dir = toUse[i + 1];
                no->esq->pai = no;
                no->dir->pai = no;
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
                no->pilha = toUse[i + 1]->pilha;
                no->esq = toUse[i];
                no->dir = toUse[i + 1];
                no->esq->pai = no;
                no->dir->pai = no;
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

        // atualiza o vetor de pais
        int toCompare = iteration == 0 ? totalElements : totalElementsNovosPais;
        if (toCompare == 1)
        {
            paiPrincipal = iteration == 0 ? pais[0] : novosPais[0];
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
 
    
    TNoA *curr = paiPrincipal;

    while (curr->info != INT_MAX)
    {
        int vencedor = paiPrincipal->info;
        // DESCE ATÉ A FOLHA DO NÓ VENCEDOR
        while (curr->esq != NULL || curr->dir != NULL)
        {
            if (curr->esq != NULL)
            {
                if (curr->esq->info == vencedor)
                {
                    curr = curr->esq;
                    continue;
                }
            }
            if (curr->dir != NULL)
            {
                if (curr->dir->info == vencedor)
                {
                    curr = curr->dir;
                    continue;
                }
            }
        }

        // salvar funcionario no arquivo
        TFunc *func = hash[vencedor];
        fseek(out, funcNoArquivo * tamanho_registro(), SEEK_SET);
        funcNoArquivo++;
        salva_funcionario(func, out);
        int stack = paiPrincipal->pilha;

        func = pop(pilha[stack], 0, &vetTop[stack]);
        // printf("Retirou da pilha o %d\n", func->cod);
        if (func == NULL)
            curr->info = INT_MAX;
        else
        {
            curr->info = func->cod;
            hash[func->cod] = func;
        }
            // print2DUtil(paiPrincipal, 0);
        // ATUALIZA A ÁRVORE DE VENCEDORES COM O NOVO VALOR E JÁ O COMPARA COM OS VALORES ALI JÁ PRESENTES
        while (curr->pai != NULL)
        {
            curr = curr->pai;
            int prevValue = curr->info;
            if (curr->esq && curr->dir)
            {
                if (curr->esq->info < curr->dir->info)
                {
                    curr->info = curr->esq->info;
                    curr->pilha = curr->esq->pilha;
                }
                else
                {
                    curr->info = curr->dir->info;
                    curr->pilha = curr->dir->pilha;
                }
            }
            else
            {
                if (curr->esq != NULL)
                {
                    curr->info = curr->esq->info;
                    curr->pilha = curr->esq->pilha;
                }
                else
                {
                    curr->info = curr->dir->info;
                    curr->pilha = curr->dir->pilha;
                }
            } 
            
        }
    }
    free(base);
    free(hash);
    free(pais);
    free(novosPais);
}
