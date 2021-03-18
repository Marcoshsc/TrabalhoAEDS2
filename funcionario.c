#include <stdlib.h>
#include <string.h>
#include "funcionarios.h"

TFunc *funcionario(int cod, char *nome, char *cpf, char *data, double salario){
	TFunc *funcionario = (TFunc *) malloc(sizeof(TFunc));
	if (funcionario) memset(funcionario, 0, sizeof(TFunc));
	funcionario->cod = cod;
	strcpy(funcionario->nome, nome);
	strcpy(funcionario->cpf, cpf);
	strcpy(funcionario->data_nascimento, data);
	funcionario->salario = salario;
	return funcionario;
}

int tamanho_registro() {
  return sizeof(TFunc);
}

void imprime_funcionario(TFunc *funcionario){
	printf("%d, %s, %s, %s, %lf \n", funcionario->cod, funcionario->nome, funcionario->cpf, funcionario->data_nascimento, funcionario->salario);
}

void salva_funcionario(TFunc *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func→nome ao invés de &func→nome, pois string já é ponteiro
    fwrite(func->nome, sizeof(char),sizeof(func->nome), out);
    fwrite(func->cpf, sizeof(char),sizeof(func->cpf), out);
    fwrite(func->data_nascimento, sizeof(char),sizeof(func->data_nascimento), out);
    fwrite(&func->salario, sizeof(double), 1, out);
    fflush(out);
}

TFunc *le_funcionario(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    int result =fread(&func->cod, sizeof(int), 1, in);
   // printf("result %d func->cod %d \n" , result, func->cod);
    if (0 >= result) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);

    return func;

}

void cria_base_dados_funcionarios(FILE* arq, FILE* arqIndicePrimario, FILE* arqIndicePrimarioNaoOrdenado, int nFunc){

    int cod;
    char nome_funcionario[TAM_NOME];
    char cpf[15];
    char data_nascimento[11];
    double salario;
    int chaves[nFunc+1];

    for (int i = 0; i < nFunc; i++) {
        printf("\n\n Informe o codigo do funcionário (%d):\n", i);
        scanf("%d", &cod);
        printf("\n\n Informe o nome do funcionário (%d):\n", i);
        scanf("%s", nome_funcionario);
        printf("Informe o cpf do funcionário:\n");
        scanf("%s", cpf);
        printf("Informe a data de nascimento do funcionário:\n");
        scanf("%s", data_nascimento);
        printf("Informe o salário do funcionário:\n");
        scanf("%lf", &salario);
        TFunc *func = funcionario(cod,nome_funcionario,cpf,data_nascimento,salario) ;
        fseek(arq, (i) * tamanho_registro(), SEEK_SET);
        salva_funcionario(func, arq);
        chaves[func->cod-1] = i;
        fwrite(&func->cod,sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
        fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
  }
   for(int f = 1 ; f <=nFunc; f++){
        fwrite(&f, sizeof(int), 1, arqIndicePrimario);
        fwrite(&chaves[f], sizeof(int), 1, arqIndicePrimario);
    }
}

void cria_base_dados_funcionarios_teste(FILE* arq, FILE* arqIndicePrimario,  FILE* arqIndicePrimarioNaoOrdenado, int nFunc){
    int i = 0;
    int chaves[nFunc+1];

    //cria arquivo para armazenar os indices primarios uma chave para cada registro de funcionario
    TFunc *func1 = funcionario(2,"Janniele","000.000.000-00","27/05/1989",5000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func1, arq);
    chaves[func1->cod]= i;
    fwrite(&func1->cod,sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func1);
    i++;
    TFunc *func3 = funcionario(1,"Italo","111.111.111-11","09/10/1986",10000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func3, arq);
    chaves[func3->cod]= i;
    fwrite(&func3->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func3);
    i++;
    TFunc *func2 = funcionario(3,"Rosangela","222.222.222-22","09/08/1970",3000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func2, arq);
    chaves[func2->cod]= i;
    fwrite(&func2->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func2);
    i++;
    TFunc *func5 = funcionario(5,"Virginia ","444.444.444-44","14/12/1990",2000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func5, arq);
    chaves[func5->cod]= i;
    fwrite(&func5->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func5);
    i++;
    TFunc *func4 = funcionario(4,"Jose Geraldo","333.333.333-33","27/05/1959",3000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func4, arq);
    chaves[func4->cod]= i;
    fwrite(&func4->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func4);
    i++;
    TFunc *func6 = funcionario(6," Adriana","555.555.555-55","26/11/1988",1000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func6, arq);
    chaves[func6->cod]= i;
    fwrite(&func6->cod ,sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func6);
    i++;
    TFunc *func8 = funcionario(8,"Dario","777.777.777-77","13/08/1989",10000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func8, arq);
    chaves[func8->cod]= i;
    fwrite(&func8->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func8);
    i++;
    TFunc *func9 = funcionario(9,"Patricia","888.888.888-88","27/05/1965",2000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func9, arq);
    chaves[func9->cod]= i;
    fwrite(&func9->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func9);
    i++;
    TFunc *func10 = funcionario(10,"Ana Beatriz ","999.999.999-99","27/05/1975",1000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func10, arq);
    chaves[func10->cod]= i;
    fwrite(&func10->cod ,sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func10);
    i++;
    TFunc *func7 = funcionario(7,"Suzanet","666.666.666-66","16/09/1979",9000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func7, arq);
    chaves[func7->cod]= i;
    fwrite(&func7->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func7);
    i++;
    TFunc *func11 = funcionario(12,"Breno","000.000.000-00","27/05/1989",5000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func11, arq);
    chaves[func11->cod]= i;
    fwrite(&func11->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func11);
    i++;
    TFunc *func13 = funcionario(11,"Pedro","111.111.111-11","09/10/1986",10000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func13, arq);
    chaves[func13->cod]= i;
    fwrite(&func13->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func13);
    i++;
    TFunc *func12 = funcionario(13,"Leandro","222.222.222-22","09/08/1970",3000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func12, arq);
    chaves[func12->cod]= i;
    fwrite(&func12->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func12);
    i++;
    TFunc *func15 = funcionario(15,"Erika ","444.444.444-44","14/12/1990",2000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func15, arq);
    chaves[func15->cod]= i;
    fwrite(&func15->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func15);
    i++;
    TFunc *func14 = funcionario(14,"Gloria","333.333.333-33","27/05/1959",3000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func14, arq);
    chaves[func14->cod]= i;
    fwrite(&func14->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func14);
    i++;
    TFunc *func16 = funcionario(16,"Ariede","555.555.555-55","26/11/1988",1000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func16, arq);
    chaves[func16->cod]= i;
    fwrite(&func16->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func16);
    i++;
    TFunc *func18 = funcionario(18,"Marcela","777.777.777-77","13/08/1989",10000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func18, arq);
    chaves[func18->cod]= i;
    fwrite(&func18->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func18);
    i++;
    TFunc *func19 = funcionario(19,"Luzia","888.888.888-88","27/05/1965",2000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func19, arq);
    chaves[func19->cod]= i;
    fwrite(&func19->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func19);
    i++;
    TFunc *func20 = funcionario(20,"Paula","999.999.999-99","27/05/1975",1000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func20, arq);
    chaves[func20->cod]= i;
    fwrite(&func20->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func20);
    i++;
    TFunc *func17 = funcionario(17,"Arnaldo","666.666.666-66","16/09/1979",9000);
    fseek(arq, (i) * tamanho_registro(), SEEK_SET);
    salva_funcionario(func17, arq);
    chaves[func17->cod]= i;
    fwrite(&func17->cod, sizeof(int), 1, arqIndicePrimarioNaoOrdenado);  //escreve a chave
    fwrite(&i, sizeof(int), 1, arqIndicePrimarioNaoOrdenado); //escreve o rrn
    free(func17);

    //escrevendo o arquivo de indice primario (chave e rrn) ordenado pela chave
    for(int f = 1 ; f <=nFunc; f++){
        fwrite(&f, sizeof(int), 1, arqIndicePrimario);  //escreve a chave
        fwrite(&chaves[f], sizeof(int), 1, arqIndicePrimario); //escreve o rrn
    }

}

void imprime_base_dados_funcionarios(FILE *arq, int nFunc) {
    rewind(arq);
    int i = 0;
    fseek(arq,  0 * sizeof (TFunc), SEEK_SET);
    if(arq != NULL) {

        int aux = 1;
        do{
            fseek(arq, (i) * tamanho_registro(), SEEK_SET);
            TFunc *func = le_funcionario(arq);
            if(func){
                imprime_funcionario(func);
                i++;
                free(func);
            }else aux = 0;
        }while(aux);
    }
    else printf("Erro ao abrir arquivo\n");
}

void imprime_indice_primario_funcionarios(FILE *arqIndicePrimario, int nFunc) {
    rewind(arqIndicePrimario);
    for(int i = 1 ; i <= nFunc ; i++){
        int chave, byteoffset;
        fread(&chave, sizeof(int), 1, arqIndicePrimario);
        fread(&byteoffset, sizeof(int), 1, arqIndicePrimario);
        printf("%d: %d\n", chave, byteoffset);
    }
}

