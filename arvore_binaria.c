#include "arvore_binaria.h"

TNoA *inicializa_arvore_binaria(void) {
    return NULL;
}

int vazia_arvore_binaria(TNoA* t){
  /* Retorna 1 se a árvore for vazia e 0 caso contrário */
  return t == NULL;

}

void imprime_arvore_binaria(TNoA *nodo) {
 printf("<"); /* notação para organizar na hora de mostrar os elementos */
  if(!vazia_arvore_binaria(nodo)) /* se a árvore não for vazia... */
  {
    /* Mostra os elementos em pré-ordem */
    printf("%d ", nodo->info); /* mostra a raiz */
    imprime_arvore_binaria(nodo->esq); /* mostra a sae (subárvore à esquerda) */
    imprime_arvore_binaria(nodo->dir); /* mostra a sad (subárvore à direita) */
  }
  printf(">"); /* notação para organizar na hora de mostrar os elementos */
}

int busca_arvore_binaria(TNoA* t, int chave) {

  if(vazia_arvore_binaria(t)) { //retorna 0 se a árvore estiver vazia
    return 0;
  }
  // O operador lógico || interrompe a busca quando o elemento for encontrado
  return t->info==chave || busca_arvore_binaria(t->esq, chave) || busca_arvore_binaria(t->dir, chave);
}

void inserir_arvore_binaria(TNoA **t , int chave, int rrn){

  /* se t aponta para null, a inserção é na raiz, senão insere os elementos de forma recursiva */
  if(*t == NULL)
  {
   // printf("no vazio\n");
    *t = (TNoA*)malloc(sizeof(TNoA)); /* Aloca memória para a estrutura */
    (*t)->esq = NULL; /* Subárvore à esquerda é NULL */
    (*t)->dir = NULL; /* Subárvore à direita é NULL */
    (*t)->info = chave; /* Armazena a informação */
    (*t)->rrn = rrn; /* Armazena a informação */
  } else {
    if(chave < (*t)->info) /* Se o número for menor então vai pra esquerda */
    {
      /* Percorre pela subárvore à esquerda */
   //    printf("chave %d < t->info %d \n", chave, (*t)->info);
      inserir_arvore_binaria(&(*t)->esq, chave,rrn);
    }
    if(chave > (*t)->info) /* Se o número for maior então vai pra direita */
    {
      /* Percorre pela subárvore à direita */
     //  printf("chave %d > t->info %d \n", chave, (*t)->info);
      inserir_arvore_binaria(&(*t)->dir, chave,rrn);
    }
  }
}

TNoA *criaNo_arvore_binaria(int chave, int rrn) {
    TNoA *novo;
    novo = (TNoA *) malloc(sizeof(TNoA));
    novo->info = chave;
    novo->rrn = rrn;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

void inserir_arvore_binaria_RAIZ(TNoA *t, int chave, int rrn){
    t = (TNoA *) malloc(sizeof(TNoA));
    t->esq = NULL;
    t->dir = NULL;
    t->info = chave;
    t->rrn = rrn;
 }

 void cria_arvore_binaria_indice_primario(FILE *arqIndicePrimario, int nFunc) {
    rewind(arqIndicePrimario);
    int chave, byteoffset;
    TNoA *raiz =inicializa_arvore_binaria();
    for(int i  = 0 ; i < nFunc ; i ++){
        fread(&chave, sizeof(int), 1, arqIndicePrimario);
        fread(&byteoffset, sizeof(int), 1, arqIndicePrimario);
       // printf("%d: %d\n", chave, byteoffset);
        inserir_arvore_binaria(&raiz,chave,byteoffset);
    }

    imprime_arvore_binaria(raiz);
}


/*métodos utilizados na árvore binária externa*/

void imprime_chaves_arvore_binaria_externa(FILE* arq_saida){
    TNoAX aux;
    rewind(arq_saida);
    printf("\n\nChave, pEsq, pDir\n");
    while(fread(&aux, sizeof(TNoAX), 1, arq_saida) == 1){
            printf("%d, %ld, %ld\n", aux.item.cod, aux.pEsq, aux.pDir);
    }
    printf("\n");
}

void inserir_recursivo_arvore_binaria_externa(long int localraiz, TFunc p, FILE* arq){

    TNoAX aux;

    if(fseek(arq, localraiz, SEEK_SET) != 0)
        printf("Erro no fseek");

    if(fread(&aux, sizeof(TNoAX), 1, arq) == 1){
        if(aux.item.cod > p.cod){ //inserção à esquerda

            if(aux.pEsq == SEM_FILHO){ //não tem filho à esquerda

                long int localpai = localraiz;

                //cria nó
                TNoAX novo;
                novo.item = p;
                novo.pEsq = SEM_FILHO;
                novo.pDir = SEM_FILHO;

                //escreve nó no fim do arquivo
                fseek(arq, 0, SEEK_END);
                fwrite(&novo, sizeof(TNoAX), 1, arq);
                //printf("\nChave do no escrito no arquiv: %d", novo.item.cod);
                long int localfilho = ftell(arq) - sizeof(TNoAX);

                //dizendo ao pai que tem um filho
                fseek(arq, localpai, SEEK_SET);
                aux.pEsq = localfilho;
                fwrite(&aux, sizeof(TNoAX), 1, arq);

            }else{ //filho à esquerda

                //insere na subárvore da esquerda
                inserir_recursivo_arvore_binaria_externa(aux.pEsq, p, arq);
            }
        }else{ //o item tem que ser inserido à direita

            if(aux.pDir == SEM_FILHO){ //não tem filho à direita

                long int localpai = localraiz;

                //cria nó
                TNoAX novo;
                novo.item = p;
                novo.pEsq = SEM_FILHO;
                novo.pDir = SEM_FILHO;

                //escreve nó no fim do arquivo
                fseek(arq, 0, SEEK_END);
                fwrite(&novo, sizeof(TNoAX), 1, arq);
               // printf("\nChave do no escrito no arquiv: %d", novo.item.cod);

                long int localfilho = ftell(arq) - sizeof(TNoAX);

                //atualizando informações do pai sobre o  filho
                fseek(arq, localpai, SEEK_SET);
                aux.pDir = localfilho;
                fwrite(&aux, sizeof(TNoAX), 1, arq);

            }else{ //filho à direita

                //insere na subárvore da direita
                inserir_recursivo_arvore_binaria_externa(aux.pDir, p, arq);
            }
        }
    }
}

void inserir_item_arvore_binaria_externa(TFunc p, FILE* arq){

    fseek(arq, 0, SEEK_END);
    long int tamanho = ftell(arq);

    if(tamanho == 0){
        TNoAX no;
        no.item = p;
        no.pEsq = SEM_FILHO;
        no.pDir = SEM_FILHO;
        fwrite(&no, sizeof(TNoAX), 1, arq);
        printf("\nChave do no escrito no arquivo: %d", no.item.cod);
    }else{
        inserir_recursivo_arvore_binaria_externa(0, p, arq);
    }
}

void criar_arvore_binaria_externa(FILE *arq_entrada, FILE*arq_saida){
    rewind(arq_entrada);
    TFunc func;
    while(fread(&func, sizeof(TFunc), 1, arq_entrada) == 1){
        inserir_item_arvore_binaria_externa(func, arq_saida);
    }
    inserir_item_arvore_binaria_externa(func, arq_saida);

    imprime_chaves_arvore_binaria_externa(arq_saida);

    int elemento = -1;

    printf("\n\nDeseja buscar no arquivo externo da árvore binária uma chave? Informe o elemento.");
    scanf("%d", &elemento);

    if(elemento!=-1){
        TFunc *f = (TFunc*)malloc(sizeof(TFunc));
        f = pesquisar_arvore_binaria_externa( elemento,arq_saida);
        printf("Funcionário %s de codigo %d encontrado! \n", f->nome, f->cod);
    }
}

TFunc* pesquisar_arvore_binaria_externa(int chave, FILE* arq){

    TFunc* resposta;

    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);

    if(tamanho == 0){
        resposta = NULL;
    }else{
        buscar_recursivo_arvore_binaria_externa(0, chave, &resposta, arq);
    }

    return resposta;
}

void buscar_recursivo_arvore_binaria_externa(long localraiz, int chave, TFunc** resposta, FILE* arq){

    TNoAX aux;
    fseek(arq, localraiz, SEEK_SET);

    if(fread(&aux, sizeof(TNoAX), 1, arq) == 1){
        if(aux.item.cod == chave){
            *resposta =  (TFunc*)malloc(sizeof(TFunc));
            **resposta = aux.item; //achou
        }else{
            if(aux.item.cod > chave){ //o item está à esquerda

                if(aux.pEsq == SEM_FILHO){ //não tem filho à esquerda

                    *resposta = NULL;

                }else{ //tem filho à esquerda

                    //busca na subárvore da esquerda
                    buscar_recursivo_arvore_binaria_externa(aux.pEsq, chave, resposta, arq);
                }
            }else{ //o item está à direita

                if(aux.pDir == SEM_FILHO){ //não tem filho à direita

                    *resposta = NULL;

                }else{ //tem filho à direita

                    //insere na subárvore da direita
                    buscar_recursivo_arvore_binaria_externa(aux.pDir, chave, resposta, arq);
                }
            }
        }
    }
}
