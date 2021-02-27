#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "algoritmoce.h"

// Preenche uma estrutura com os progenitores da pr�xima gera��o, de acordo com o resultados do torneio
// Par�metros de entrada: popula��o actual (pop), estrutura com par�metros (d) e popula��o de pais a encher
void tournament_geral(pchrom pop, info d, pchrom parents)
{
	int i, j, k, sair, best, * pos;

	pos = malloc(d.tsize * sizeof(int));
	// Realiza popsize torneios
	for (i = 0; i < d.popsize; i++)
	{
		// Seleciona tsize solu��es diferentes para entrarem em torneio de sele��o
		for (j = 0; j < d.tsize; j++)
		{
			do
			{
				pos[j] = random_l_h(0, d.popsize - 1);
				// Verifica se a nova posi��o escolhida � igual a alguma das outras posi��es escolhidas
				sair = 0;
				for (k = 0; k < j; k++)
				{
					if (pos[k] == pos[j])
						sair = 1;
				}
			} while (sair);
			// Guarda a posi��o da melhor solu��o de todas as que entraram em torneio
			if (j == 0 || pop[pos[j]].fitness < pop[pos[best]].fitness)		// Problema de maximizacao
				best = j;
		}
		parents[i] = pop[pos[best]];
	}
	free(pos);
}

// Operadores geneticos a usar na gera��o dos filhos
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void genetic_operators(pchrom parents, info d, pchrom offspring, int opt)
{
	// Recombina��o por ordem e mutacoes
	order_crossover(parents, d, offspring);
	if(opt == 1)		mutacao_troca(offspring, d);
	else if(opt == 2)	mutacao_insercao(offspring, d);
	else if(opt == 3)	mutacao_inversao(offspring, d);
}

//Recebe: Pai 1 (p1), Pai 2 (p2), Descendente 1(d1), Descendente 2(d2) e por fim, os parametros (d)
//Esta fun��o encarrega-se de fazer a recombina��o.
void cx_order(int p1[], int p2[], int d1[], int d2[], info d) {
	int point1, point2, index, i, j, k, temp, avanca = 0;
	int sair = 0;

	//Pontos de corte onde vamos obter o intervalo que vai ser transposto para outras fun��es.
	point1 = random_l_h(0, d.numGenes - 1);
	do {
		point2 = random_l_h(0, d.numGenes - 1);
	} while (point2 == point1);
	if (point2 < point1) {
		temp = point2;
		point2 = point1;
		point1 = temp;
	}

	//Vamos colocar nos descendentes os n�meros que se encontram no intervalo
	//(Esses n�meros v�o para as mesmas posi��es em que se encontravam nos parentes)
	for (i = point1; i <= point2; i++) {
		d1[i] = p2[i];
		d2[i] = p1[i];
	}
	
	// preencher o resto do descendente 1
	index = point2 - point1;
	for (i = 0 , j = 0; i < d.numGenes && j < d.numGenes; i++) {
		for (k = point1; k <= point2; k++) {		//Vamos analisar se o n�mero encontrado no progenitor
			if (p1[i] == d1[k]) {					//� igual a algum dos n�meros que se encontram no intervalo que foi posto no descendente
				sair = 1;							//Caso exista, ativamos a flag e saimos imediatamente do ciclo
				break;
			}
		}
		if (sair) {									//Este 'if' vai verificar se a flag est� ativa
			sair = 0;								//Se estiver ativa, voltamos a colocar a flag a 0 e passamos para o pr�ximo n�vel.
			continue;
		}
		if (j == point1) {							//Este 'if' � feito de modo a prevenir que n�o se alteram os valores do intervalo que
			j += (index + 1);						//que foram inicialmente l� postos
		}
		d1[j] = p1[i];								//Se o n�mero que se encontra no progenitor "passar" em todos os testes � ent�o
		j++;										//colocado na descendencia.
	}

	//Preenchimento do Descendente2 (da mesma forma)
	for (i = 0, j = 0; i < d.numGenes && j < d.numGenes; i++) {
		for (k = point1; k <= point2; k++) {
			if (p2[i] == d2[k]) {
				sair = 1;
				break;
			}
		}
		if (sair) {
			sair = 0;
			continue;
		}
		if (j == point1) {
			j += (index + 1);
		}
		d2[j] = p2[i];
		j++;
	}
}

// Preenche o vector descendentes com o resultado das opera��es de recombina��o
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void order_crossover(pchrom parents, info d, pchrom offspring)
{
	int i;
	// Recombina��o por ordem
	for (i = 0; i < d.popsize; i += 2) {
		if (rand_01() < d.pr) {

			cx_order((parents + i)->solInd, (parents + i + 1)->solInd, (offspring + i)->solInd, (offspring + i + 1)->solInd, d);
		}

		else {
			offspring[i] = parents[i];
			offspring[i + 1] = parents[i + 1];
		}
	}
}

// Muta��o por troca (troca de dois pontos)
// Par�metros de entrada: estrutura com os descendentes (offspring) e estrutura com par�metros (d)
void mutacao_troca(pchrom offspring, info d)
{
	int i, j, mutacao1, mutacao2,temp;

	for (i = 0; i < d.popsize; i++)
		for (j = 0; j < d.numGenes; j++)
			if (rand_01() < d.pm) {
				mutacao1 = random_l_h(0, d.numGenes-1);
				do {
					mutacao2 = random_l_h(0, d.numGenes-1);
				} while (mutacao1 == mutacao2);

				temp = offspring[i].solInd[mutacao1];
				offspring[i].solInd[mutacao1] = offspring[i].solInd[mutacao2];
				offspring[i].solInd[mutacao2] = temp;
			}
} 

// Muta��o por inser��o (pegar num ponto e coloc�-lo num s�tio mais � frente, obrigando o resto dos pontos
//						 a colocarem-se em posi��es mais para tr�s)
// Par�metros de entrada: estrutura com os descendentes (offspring) e estrutura com par�metros (d)
void mutacao_insercao(pchrom offspring, info d) {
	int i, j, escolhido, posInserir, temp;
	
	for (i = 0; i < d.popsize; i++)
		for (j = 0; j < d.numGenes; j++)
			if (rand_01() < d.pm) {
				escolhido = random_l_h(0, d.numGenes - 2);
				do {
					posInserir = random_l_h(1, d.numGenes - 1);
				} while(posInserir <= escolhido);
				
				temp = offspring[i].solInd[escolhido];
				for (j = escolhido; j < posInserir; j++) {
					offspring[i].solInd[j] = offspring[i].solInd[j + 1];
				}
				offspring[i].solInd[posInserir] = temp;
			}
}

// Muta��o por invers�o (inverter 2 posi��es, ou seja, pega num ponto e vai troc�-lo de posi��o com outro ponto
//						 que est� imediatamente � sua direita)
// Par�metros de entrada: estrutura com os descendentes (offspring) e estrutura com par�metros (d)
void mutacao_inversao(pchrom offspring, info d) {
	int i, j, inverte, temp;
	
	for (i = 0; i < d.popsize; i++)
		for (j = 0; j < d.numGenes; j++)
			if (rand_01() < d.pm) {
				inverte = random_l_h(0, d.numGenes - 2);
				temp = offspring[i].solInd[inverte];
				offspring[i].solInd[inverte] = offspring[i].solInd[inverte + 1];
				offspring[i].solInd[inverte + 1] = temp;
			}
}

