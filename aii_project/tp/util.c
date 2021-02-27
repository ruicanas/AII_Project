#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h"

#define MAXL 1000
#define POP 100
#define PROBMUT 0.01
#define PROBRE 0.3
#define TSIZE 4
#define MAXGEN 2500


// Leitura do ficheiro de input
// Recebe: nome do ficheiro, numero de vertices (ptr), numero de arestas (ptr)
// Devolve uma matriz do mesmo tipo que está no ficheiro de texto
int* init_dados(char* nome, int* vertices, int* arestas)
{
	FILE* f;
	int* p, * q;
	int i, j, contaLinha = 0;
	char linha[MAXL];

	f = fopen(nome, "r");
	if (!f)
	{
		printf("Erro no acesso ao ficheiro dos dados\n");
		exit(1);
	}
	 

	// Temos uma linha em comentário que não vai ser lida e valores que devem ser ignorados
	// Desta forma, vamos buscar linha a linha ao ficheiro de texto e fazer-lhe uma análise
	while (fgets(linha, MAXL, f)) {
		int length = (int)strlen(linha);
		if (linha[0] == '%' && linha[1] == '%') {  //Verifica se a linha obtida é um comentário ou não
			break;
		}
	}

	fscanf(f, "%d %d %d", vertices, vertices, arestas); 
		

		// Alocacao dinamica da matriz
		p = malloc(sizeof(int) * (*arestas)*2);
		if (!p)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}

		q = p;
		// Preenchimento da matriz, colocando os valores 1 a 1.
		for (i = 0; i < *arestas; i++) 
			for (j = 0; j < 2; j++) 
				fscanf(f, " %d", q++);
			
	
	fclose(f);
	return p;
}

//Recebe: o número de vertices e o número de arestas uma vez que poderão ser úteis
//É feito o preenchimento dos parametros manualmente. 
//Devolve: os parametros já preeenchidos
info preenchePara(int vertices, int arestas) {
	info para;

	para.popsize = POP;
	para.pm = PROBMUT;
	para.pr = PROBRE;
	para.numGenerations = MAXGEN;
	para.numGenes = vertices;
	para.tsize = TSIZE;
	para.ar = arestas;
	para.ro = 0.0;
	return para;
}


// Criacao da populacao inicial. O vector e alocado dinamicamente
// Parâmetro de entrada: Estrutura com parâmetros do problema
// Parâmetro de saída: Preenche da estrutura da população apenas o vector binário com os elementos.
pchrom init_pop(info d)
{
	int     i, j ,k , numgerado;
	pchrom  indiv;

	indiv = malloc(sizeof(chrom) * d.popsize);
	if (indiv == NULL)
	{
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}

	for (i = 0; i < d.popsize; i++)
	{
		j = 0;
		do {
			int sai = 0;
			for (j; j < d.numGenes; j++) {
				numgerado = random_l_h(1, d.numGenes);
				for (k = 0; k < d.numGenes; k++) {
					if (numgerado == indiv[i].solInd[k]) {
						sai = 1;
						break;
					}
				}
				if (sai) {
					break;
				}
				indiv[i].solInd[j] = numgerado;
			}
		} while (j != d.numGenes);
	}
	return indiv;
}


// Gera a solucao inicial
// Parametros: solucao, numero de vertices
void gera_sol_inicial(int* sol, int v)
{
	int i = 0, j;
	int numgerado;

	for (i = 0; i < v; i++) {
		sol[i] = 0;
	}

	//Para evitar mecanismos de reparação e de penalização
	//Preparamos a solução inicial para que não seja inválida
	i = 0;
	do {
		int sai = 0;
		for (i ; i < v; i++) {
			numgerado = random_l_h(1, v);
			for (j = 0; j < v; j++) {
				if (numgerado == sol[j]){
					sai = 1;
					break;
				}
			}
			if (sai) {
				break;
			}
			sol[i] = numgerado;
		}
	} while (i != v);
}

// Escreve solucao
// Parametros: solucao e numero de vertices
void escreve_sol(int* sol, int vert)
{
	int i;

	printf("\nSolucao: ");
	for (i = 0; i < vert; i++)
			printf("%2d  ", sol[i]);

	printf("\n");
}

// copia vector b para a (tamanho n)
void substitui(int a[], int b[], int n)
{
	int i;
	for (i = 0; i < n; i++)
		a[i] = b[i];
}

// Inicializa o gerador de numeros aleatorios
void init_rand()
{
	srand((unsigned)time(NULL));
}

// Devolve valor inteiro aleatorio entre min e max
int random_l_h(int min, int max)
{
	return min + rand() % (max - min + 1);
}

// Devolve um valor real aleatorio do intervalo [0, 1]
float rand_01()
{
	return ((float)rand()) / RAND_MAX;
}


// Simula o lançamento de uma moeda, retornando o valor 0 ou 1
int flip()
{
	if ((((float)rand()) / RAND_MAX) < 0.5)
		return 0;
	else
		return 1;
}

// Actualiza a melhor solução encontrada
// Parâmetro de entrada: populacao actual (pop), estrutura com parâmetros (d) e a melhor solucao encontrada até a geração imediatamente anterior (best)
// Parâmetro de saída: a melhor solucao encontrada até a geração actual
chrom get_best(pchrom pop, info d, chrom best)
{
	int i;

	for (i = 0; i < d.popsize; i++)
	{
		if (best.fitness > pop[i].fitness && pop[i].valido == 1) // Queremos um fitness o mais baixo possível (o nosso fitness acaba por ser o custo)
			best = pop[i];										 // e válido!
	}
	return best;
}

//Recebe: a população (pop) e os respetivos parametros dessa mesma população (d)
//Esta função foi feita de forma a testar se todas as soluções de uma população seriam válidas
chrom escolha_inicial(pchrom pop, info d) {
	chrom dft;
	int i;

	//Vai à procura de uma solução com o fitness diferente de 0 e que seja válido
	//(Caso fosse inválida o fitness seria 0 e naturalmente, seria inválida)
	for (i = 0; i < d.popsize; i++) {
		if (pop[i].fitness != 0 && pop[i].valido == 1) {
			return pop[i];
		}
	}
	dft.fitness = 999999; //Caso fossem todas inválidas(o que não seria de esperar) foi feito um mecanismo que iria devolver
	return dft;			  //um valor alto(que nos iria dar a indicação de que a população teria todas as soluções inválidas)
}
//Esta função foi mantida uma vez que foi extremamente útil da nossa parte para interpretar o programa, principalmente 
//na parte dos operadores genéticos


// Escreve uma solução na consola
// Parâmetro de entrada: populacao actual (pop) e estrutura com parâmetros (d)
void write_best(chrom x, info d)
{
	int i;
	if (x.fitness < 999999) {
		printf("\nBest individual: %4d\n", x.fitness);
		for (i = 0; i < d.numGenes; i++)
			printf("%4d", x.solInd[i]);
		putchar('\n');
	}
	else {
		printf("\nNao existe best individual porque sao todos invalidos!");
	}
}


