#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "funcao.h"
#include "util.h"
#include <math.h>

// Gera um vizinho que vai trocar de posição só com aqueles que tiverem a sua paridade
// por exemplo, o número 5 só vai trocar com o 3 e com o 1
// Parametros de entrada: Solucao actual, a, Solucao vizinha, b, Numero de vertices, n
void gera_vizinho3(int a[], int b[], int n) {
	int i, j, temp;
	// Copia a solucao atual para a solucao vizinha
	for (i = 0; i < n; i++)
		b[i] = a[i];

	for (i = 0; i < n; i++) {
		if (b[i] % 2 == 0) {
			temp = b[i];
			do {
				j = random_l_h(0, n-1);
			} while (b[j] % 2 != 0 || j == i);
			b[i] = b[j];
			b[j] = temp;
		}
		else {
			temp = b[i];
			do {
				j = random_l_h(0, n-1);
			} while (b[j] % 2 == 0 || j == i);
			b[i] = b[j];
			b[j] = temp;
		}
	}
}

// Gera um vizinho à distância 2 (troca dois vértices de cada grupo)
// Parâmetros de entrada: Solução actual, a, Solução vizinha, b, Número de vértices, n
void gera_vizinho2(int a[], int b[], int n)
{
	int i, p1, p2, p3, p4, temp1, temp2;


	//Copiar a solução atual para a solução vizinha
	for (i = 0; i < n; i++)
		b[i] = a[i];

	
	p1 = random_l_h(0, n - 1);
	do {
		p2 = random_l_h(0, n - 1);
	} while (p1 == p2);

	do {
		p3 = random_l_h(0, n - 1);
	} while (p3 == p1);

	do {
		p4 = random_l_h(0, n - 1);
	} while (p4 == p3 || p4 == p2);

	// Troca as posicoes do vetor da solução
	temp1 = b[p1];
	temp2 = b[p3];
	b[p1] = b[p2];
	b[p2] = temp1;
	b[p3] = b[p4];
	b[p4] = temp2;
}

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
//swap two vertices
void gera_vizinho(int a[], int b[], int n)
{
	int i, p1, p2, temp;


	//Copiar a solução atual para a solução vizinha
	for (i = 0; i < n; i++)
		b[i] = a[i];

	
	p1 = random_l_h(0, n - 1);
	do {
		p2 = random_l_h(0, n - 1);
	} while (p1 == p2);
	
	// Troca as posicoes do vetor da solução
	temp = b[p1];
	b[p1] = b[p2];
	b[p2] = temp;
}

// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices, numero de iteracoes e arestas
// (Arestas vão nos ser úteis para o calcula_fit)
// Devolve o custo da melhor solucao encontrada
int trepa_colinas(int sol[], int* mat, int vert, int num_iter, int arest, int op)
{
	int* nova_sol, custo, custo_viz, i;


	nova_sol = malloc(sizeof(int) * vert);
	if (nova_sol == NULL)
	{
		printf("Erro na alocacao de memoria");
		exit(1);
	}
	// Avalia solucao inicial
	custo = calcula_fit(sol, mat, vert, arest);
	
	for (i = 0; i < num_iter; i++)
	{
		// Gera vizinho
		if(op==1)gera_vizinho(sol, nova_sol, vert);
		else if (op==2)gera_vizinho2(sol, nova_sol, vert);
		else gera_vizinho3(sol, nova_sol, vert);
		// Avalia vizinho
		custo_viz = calcula_fit(nova_sol, mat, vert, arest);
		
		if (custo_viz <= custo)					//Não esquecer fazer com o =
		{
			substitui(sol, nova_sol, vert);
			custo = custo_viz;
		}
	}
	free(nova_sol);
	return custo;
}