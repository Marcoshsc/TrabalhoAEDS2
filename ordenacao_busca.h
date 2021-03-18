#ifndef ORDENACAO_BUSCA_H
#define ORDENACAO_BUSCA_H

#include <stdio.h>
#include "funcionarios.h"

TFunc* busca_binaria(int cod, FILE *arq, int tam);

void insertion_sort_disco(FILE *arq, int tam);

#endif // ORDENACAO_BUSCA_H

