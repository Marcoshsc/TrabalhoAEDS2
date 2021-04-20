

#include "arvore_b+.h"
#include "string.h"
#include "math.h"

TNoB *criaNo_arvore_B(char separador[])
{
    TNoB *novo;
    novo = (TNoB *)malloc(sizeof(TNoB));
    strcpy(novo->separador, separador);
    novo->pai = (TNoB *)malloc(sizeof(TNoB *));
    novo->esq = (TNoB *)malloc(sizeof(TNoB *));
    novo->dir = (TNoB *)malloc(sizeof(TNoB *));
    novo->rrn = -1;
    novo->bloco = NULL;
    strcpy(novo->leaf, FALSE);
    return novo;
}

void freeNoB(TNoB *no)
{
    free(no->pai);
    free(no->esq);
    free(no->dir);
}

void inserir_arvore_B_RAIZ(TNoB *t, char separador[], int rrn)
{
    t = (TNoB *)malloc(sizeof(TNoB));
    t->esq = NULL;
    t->dir = NULL;
    strcpy(t->separador, separador);
    t->rrn = rrn;
}

// key1, key e sep apontam para o começo do array de caracteres
void FindSeparator(char *key1, char *key2, char *sep)
{
    while (1)
    { //loop until break
        *sep = *key2;
        //printf("\nKey1: %s - Key2: %s - Sep: %s\n", key1, key2, sep);
        sep++; //move o caractere corrente para sep
        if (*key2 != *key1)
            break; //para quando uma diferença é encontrada
        if (*key2 == 0)
            break; //para no final de key2
        key1++;
        key2++; //move para o próximo caractere das chaves
    }
    *sep = 0; //null para terminar o separador de string
}

int estaNoArray(int n, int array[], int tam)
{

    for (int i = 0; i < tam; i++)
    {
        if (n == array[i])
        {
            return 1;
        }
    }
    return 0;
}

void ordemAlfabetica(FILE *arq, int nFunc, int *ordem[])
{

    TFunc *v = malloc(nFunc * sizeof(TFunc *));
    int reg = 0;
    char menor[50];
    char *atual;
    int menores[nFunc];
    int position = 0;
    int i = 0;

    fseek(arq, (reg)*tamanho_registro(), SEEK_SET);
    v = le_funcionario(arq);
    strcpy(menor, v->nome);

    int j = 0;
    reg = 1;

    while (i < nFunc)
    {
        rewind(arq);
        j = 0;

        while (1)
        {

            if (estaNoArray(reg, menores, i))
            {
                reg++;
                continue;
            }

            fseek(arq, (reg)*tamanho_registro(), SEEK_SET);
            v = le_funcionario(arq);

            if (v == NULL)
            {
                break;
            }

            int tam = 0;

            while (v->nome[tam] != '\0' || menor[tam] != '\0')
            {
                if (v->nome[tam] == menor[tam])
                {
                    tam++;
                }
                else if (v->nome[tam] < menor[tam])
                {
                    strcpy(menor, v->nome);
                    position = reg;
                    break;
                }
                else
                {
                    break;
                }
            }

            reg++;
            j++;
        }

        menores[i] = position;
        strcpy(menor, "zzz");
        position = 0;
        i++;
        reg = 0;
    }

    int k;

    for (k = 0; k < nFunc; k++)
    {
        ordem[k] = menores[k];
    }

    free(v);
}
Lista *BlocosEncadeados(FILE *arq, int nFunc)
{
    Lista *blocos;
    TFunc *func1;
    int *ordem[nFunc];
    int i;
    int position;

    ordemAlfabetica(arq, nFunc, ordem);

    rewind(arq);
    blocos = cria("Head", NULL);

    //insere as chaves de forma ordenada;
    for (i = nFunc - 1; i >= 0; i--)
    {

        rewind(arq);
        position = ordem[i];
        fseek(arq, position * tamanho_registro(), SEEK_SET);
        func1 = le_funcionario(arq);
        blocos = cria(func1->nome, blocos);
        blocos->tamanho = position;
    }

    return blocos;
}

int isLeaf(TNoB *no)
{

    if (no->rrn != -1)
    {
        return no->rrn;
    }
    return -1;
}

void imprimeNoB(TNoB *no)
{
    printf("Bloco: %d\n", no->bloco);
    printf("Ponteiro: %s\n", no->separador);
    printf("Pai: %s\n", no->pai->separador);
    printf("Folha: %s\n", no->leaf);
    printf("rnn: %d\n\n", no->rrn);
}
void criaIndice(FILE *arq, TNoB **nos, int tam)
{

    int i;
    for (i = 0; i < tam; i++)
    {
        fwrite(&nos[i]->bloco, sizeof(int), 1, arq);
        fwrite(nos[i]->separador, sizeof(char), sizeof(nos[i]->separador), arq);
        fwrite(nos[i]->pai->separador, sizeof(char), sizeof(nos[i]->pai->separador), arq);
        fwrite(nos[i]->leaf, sizeof(char), sizeof(nos[i]->leaf), arq);
        fwrite(&nos[i]->esq->rrn, sizeof(int), 1, arq);
        fflush(arq);
    }
}

void imprimeIndice(char *nomeArq, int tam)
{
    FILE *arq = fopen(nomeArq, "rb");
    TNoB *no = criaNo_arvore_B("null");
    int i;

    rewind(arq);
    while (!feof(arq))
    {
        fread(&no->bloco, sizeof(int), 1, arq);
        fread(no->separador, sizeof(char), sizeof(no->separador), arq);
        fread(no->pai->separador, sizeof(char), sizeof(no->pai->separador), arq);
        fread(no->leaf, sizeof(char), sizeof(no->leaf), arq);
        fread(&no->rrn, sizeof(int), 1, arq);

        imprimeNoB(no);
    }

    free(no);
    fclose(arq);
}
void criaBMais(char *nome_arquivo, int M, int nFunc)
{

    int blocSize = nFunc / 2;
    FILE *arq;
    FILE *indexArq = fopen("indice.dat", "wb+");
    Lista *blocos;
    if ((arq = fopen(nome_arquivo, "rb")))
    {
        printf(" ------> Árvore B+ <-------\n");
    }
    else
    {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    int i;
    int j = 1;
    int contEl = 0;

    blocos = BlocosEncadeados(arq, nFunc);
    TNoB **nos = malloc(blocSize * sizeof(TNoB *));
    TNoB **nosPais = malloc(blocSize * sizeof(TNoB *));

    char *key1;
    char *key2;
    char sep[15];
    int totalElements = 0;
    int rrnE, rrnD;
    int contB = 1;
    //imprimeBlocos(blocos);

    //Cria Nós Folhas
    while (blocos->prox->prox->tamanho != -1)
    {

        key1 = blocos->prox->nome;
        rrnE = blocos->tamanho;

        blocos = blocos->prox->prox;

        key2 = blocos->nome;
        rrnD = blocos->tamanho;

        FindSeparator(key1, key2, sep);
        nos[totalElements] = criaNo_arvore_B(sep);
        strcpy(nos[totalElements]->esq->separador, key1);
        nos[totalElements]->esq->rrn = rrnE;
        strcpy(nos[totalElements]->dir->separador, key2);
        nos[totalElements]->dir->rrn = rrnD;
        nos[totalElements]->bloco = totalElements;
        strcpy(nos[totalElements]->leaf, TRUE);

        if (contB == M)
        {
            nos[totalElements - 1]->dir = nos[totalElements];
            contB = 0;
        }
        else
        {
            contB++;
        }
        totalElements++;
    }

    int cont = 0;

    // Cria Nós pais
    int rrn;
    for (i = 0; i < totalElements; i++)
    {

        if (cont == M)
        {
            strcpy(nos[i]->leaf, FALSE);
            nos[i]->esq = nos[i - 2];
            nos[i]->esq->pai = nos[i];

            if (nos[i + 1] != NULL)
            {

                nos[i]->dir = nos[i + 1];
                nos[i]->dir->pai = nos[i];
            }
            else
            {

                nos[i]->dir->pai = nos[i];
                nos[i]->dir->pai->rrn = nos[i]->dir->rrn;
            }
            i + 1;
            cont++;
            continue;
        }
        else if (cont == (2 * M + 1))
        {

            strcpy(nos[i]->leaf, FALSE);
            nos[i]->esq = nos[i - 3];
            nos[i]->esq->pai = nos[i];

            if (totalElements < i + 3)
            {

                nos[i]->dir = nos[i + 3];
                nos[i]->dir->pai = nos[i];
            }
            else
            {
                nos[i]->dir = nos[totalElements - 1];
                nos[i]->dir->pai = nos[i];
            }
            cont = 0;
            continue;
        }

        cont++;
    }

    printf("\n\nCRIA INDICE -----\n");

    criaIndice(indexArq, nos, totalElements);

    printf("\n\nIMPRIME INDICE -----\n\n");

    imprimeIndice("indice.dat", totalElements);

    libera(blocos);
    fclose(arq);
    fclose(indexArq);
    free(nos);
}