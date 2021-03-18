#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Funcionario {
  int cod;
  char nome[50];
  char cpf[15];
  char data_nascimento[11];
  double salario;
} TFunc;

void salva(TFunc *func, FILE *out);
int tamanho_registro();
TFunc *le(FILE *in);
int tamanho_arquivo(FILE *arq);
TFunc* busca_binaria(int cod, FILE *arq, int tam);
void initialize(FILE* file, int numberRecords);
void print(TFunc func);

int main_bb() {

  FILE* file = fopen("funcionarios.dat", "w+");

  if(file == NULL) {
    printf("Não foi possível abrir o arquivo.\n");
    return 1;
  }

  int numberRecords = 10000;

  initialize(file, numberRecords);

  TFunc* func = busca_binaria(305, file, numberRecords);
  if(func == NULL)
    printf("Funcionário não encontrado.\n");
  else
    print(*func);

  free(func);
  fclose(file);

}

TFunc* busca_binaria(int cod, FILE *arq, int tam) {

  int left = 0, right = tam - 1;
  while(left <= right)
  {
    int middle = (left + right) / 2;
    fseek(arq, middle * tamanho_registro(), SEEK_SET);
    TFunc* func = le(arq);
    if(cod == func->cod) {
      return func;
    }
    else if(func->cod < cod) {
      left = middle + 1;
    }
    else {
      right = middle - 1;
    }
  }

  return NULL;
}

void print(TFunc func) {
  printf("\nCodigo: %d", func.cod);
  printf("\nCPF: %s", func.cpf);
  printf("\nNome: %s", func.nome);
  printf("\nData nascimento: %s", func.data_nascimento);
  printf("\nSalário: %.2f", func.salario);
  printf("\n");
}

void initialize(FILE* file, int numberRecords) {
  for (int i = 1; i <= numberRecords; i++) {
    TFunc func;
    func.cod = i;
    sprintf(func.data_nascimento, "01/01/2000");
    sprintf(func.cpf, "111.111.111-11");
    sprintf(func.nome, "Funcionario %d", i);
    func.salario = 1000.2;
    fseek(file, (i - 1) * tamanho_registro(), SEEK_SET);
    salva(&func, file);
  }

}

void salva(TFunc *func, FILE *out) {
  fwrite(&func->cod, sizeof(int), 1, out);
  fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
  fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
  fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
  fwrite(&func->salario, sizeof(double), 1, out);
}

int tamanho_registro() {
  return sizeof(TFunc);
}

TFunc *le(FILE *in) {
  TFunc *func = (TFunc *) malloc(sizeof(TFunc));
  if(0 >= fread(&func->cod, sizeof(int), 1, in)) {
    free(func);
    return NULL;
  }
  fread(func->nome, sizeof(char), sizeof(func->nome), in);
  fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
  fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
  fread(&func->salario, sizeof(double), 1, in);
  return func;
}

int tamanho_arquivo(FILE *arq) {
  fseek(arq, 0, SEEK_END);
  int tam = trunc(ftell(arq) / tamanho_registro());
  return tam;
}
