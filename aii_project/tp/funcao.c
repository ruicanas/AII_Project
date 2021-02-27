#define _CRT_SECURE_NO_WARNINGS 1
#include "funcao.h"
#include <stdlib.h>

// Calcula a qualidade de uma solu��o
// Recebe:  A solu��o, a, a matriz de adjac�ncias, mat, o n�mero de v�rtices, vert e o n�mero de arestas, arest
// Devolve: O valor mais alto encontrado, de acordo com o que � pedido no enunciado (Problema de maximiza��o)
int calcula_fit(int a[], int* mat, int vert, int arest)
{	
	int best = 0;
	int conta = 0;
	int total = 0;
	int valor1, valor2;
	int i, j;

	//Vamos correr a nossa matriz linha a linha (dai a necessidae das arestas)
	//porque para tirarmos as nossas conclus�es � preciso fazer os c�lculos para todos os v�rtices.
	//Uma vez que o valor procurado vai ter que ser o maior poss�vel que se encontre de entre os c�lculos
	//Vamos guardando um valor sempre que o mesmo seja maior que o valor calculado anteriormente.
	//Este processo � feito at� chegarmos ao fim da matriz.
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
