#define _CRT_SECURE_NO_WARNINGS 1
#include "funcao.h"
#include <stdlib.h>

// Calcula a qualidade de uma solução
// Recebe:  A solução, a, a matriz de adjacências, mat, o número de vértices, vert e o número de arestas, arest
// Devolve: O valor mais alto encontrado, de acordo com o que é pedido no enunciado (Problema de maximização)
int calcula_fit(int a[], int* mat, int vert, int arest)
{	
	int best = 0;
	int conta = 0;
	int total = 0;
	int valor1, valor2;
	int i, j;

	//Vamos correr a nossa matriz linha a linha (dai a necessidae das arestas)
	//porque para tirarmos as nossas conclusões é preciso fazer os cálculos para todos os vértices.
	//Uma vez que o valor procurado vai ter que ser o maior possível que se encontre de entre os cálculos
	//Vamos guardando um valor sempre que o mesmo seja maior que o valor calculado anteriormente.
	//Este processo é feito até chegarmos ao fim da matriz.
	for (i = 0; i < arest; i++) {
		for (j = 0; j < 2; j++) {
			if (j == 1) {
				valor1 = *(mat + i * 2 + j);
			}
			else {
				valor2 = *(mat + i * 2 + j);
			}
		}
		total = abs(a[valor2 - 1] - a[valor1 - 1]);
		if (total > best)
			best = total;
	}

	return best;
}
