#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "algoritmo.h"
#include "algoritmoce.h"
#include "funcaoce.h"

void fazPesquisaLocal();
void fazPesquisaEvolutiva();
void fazPesquisaHibrido();

int main(int argc, char* argv[]) {
	int op;

	do {
		printf("\tPrograma feito por Rui Canas(21280738) - P1 e Tiago Oliveira(21280739) - P2\n\t\t\tIntroducao a Inteligencia Artificial\n\n");
		printf("\t\t\t#     # ####### #     # #     #\n");
		printf("\t\t\t##   ## #       ##    # #     #\n");
		printf("\t\t\t# # # # #       # #   # #     #\n");
		printf("\t\t\t#  #  # #####   #  #  # #     #\n");
		printf("\t\t\t#     # #       #   # # #     #\n");
		printf("\t\t\t#     # ####### #     #  #####  \n");

	printf("\n\t\t\t1 - Algoritmo de Pesquisa local\n\t\t\t2 - Algoritmo Evolutivo\n\t\t\t3 - Algoritmo Hibrido\n\t\t\t0 - Sair\n");
	printf("\n\t\t\tOpcao: ");
	scanf("%d", &op);
	
		switch (op) {
		case(1): fazPesquisaLocal();
				 break;
		case (2): fazPesquisaEvolutiva();
			break;
		case (3): fazPesquisaHibrido();
			break;
		default:
			if (op != 0)
				printf("Opcao Invalida!\n\n");
			break;
		}
	} while (op != 0);

	return 0;
}

//Apenas para dar detalhes ao utilizador acerca do nome do ficheiro assim como das repetições 
void info_expAtual(char nome_fich[], int reps, int num_iter) {
	system("cls");
	if(num_iter != 0)
	printf("Nome do fich: %s\nReps: %d\nIteracoes: %d\n", nome_fich, reps, num_iter);
	else
	printf("Nome do fich: %s\nReps: %d\n", nome_fich, reps);

}

void fazPesquisaLocal() {
	char    nome_fich[100];
	int		reps, num_iter, vert, arest, custo, best_custo, i, subopt;
	int*	grafo, * sol, * best;
	float	mbf = 0.0;

	system("cls");
	printf("Nome do ficheiro: ");
	scanf(" %99[^\n]", nome_fich);
	printf("\nRepeticoes: ");
	scanf("%d", &reps);
	printf("\nNumero de Iteracoes: ");
	scanf("%d", &num_iter);

	do {
		system("cls");
		printf("Escolha o operador genetico de mutacao:\n");
		printf("1 - Gera-Vizinhanca(troca 2)\n2 - Gera-Vizinhanca(troca 4)\n3 - Gera-Vizinhanca(Troca com igual paridade)\nOpcao: ");
		scanf("%d", &subopt);
	} while (subopt <= 0 || subopt >= 4);

	init_rand();
	grafo = init_dados(nome_fich, &vert, &arest);
	sol = malloc(sizeof(int) * vert);
	best = malloc(sizeof(int) * vert);
	if (sol == NULL || best == NULL)
	{
		printf("Erro na alocacao de memoria");
		exit(1);
	}

	info_expAtual(nome_fich, reps, num_iter);
	for (i = 0; i < reps; i++) {
		// Gerar solucao inicial
		gera_sol_inicial(sol, vert);
		escreve_sol(sol, vert);

		custo = trepa_colinas(sol, grafo, vert, num_iter, arest, subopt);

		printf("\nRepeticao %d:", i);
		escreve_sol(sol, vert);
		printf("Custo final: %2d\n", custo);
		mbf += custo;
		if (i == 0 || best_custo > custo)
		{
			best_custo = custo;
			substitui(best, sol, vert);
		}
	}
	// Escreve eresultados globais
	printf("\n\nMBF: %f\n", mbf / i);
	printf("\nMelhor solucao encontrada");
	escreve_sol(best, vert);
	printf("Custo final: %2d\n", best_custo);

	free(grafo);
	free(sol);
	free(best);

} //Faltam os diferentes tipos de 

void fazPesquisaEvolutiva() {
	char		nome_fich[100];
	int			reps, vert, arest, i, flag, auxf, gen_actual, fitnessIgual = 0;
	int			subopt;
	pchrom      pop = NULL, parents = NULL;
	chrom       best_run, best_ever;
	int*		grafo;
	float		mbf = 0.0;
	info		parametros;

	system("cls");
	printf("Nome do ficheiro: ");
	scanf(" %99[^\n]", nome_fich);
	printf("\nRepeticoes: ");
	scanf("%d", &reps);

	do {
		system("cls");
		printf("Escolha o operador genetico de mutacao:\n");
		printf("1 - Troca\n2 - Insercao\n3 - Inversao\nOpcao: ");
		scanf("%d", &subopt);
	} while (subopt <= 0 || subopt >= 4);
	
	init_rand();
	grafo = init_dados(nome_fich, &vert, &arest);
	parametros = preenchePara(vert,arest);
	info_expAtual(nome_fich, reps, 0);

	for (i = 0; i < reps; i++)
	{
		printf("Repeticao %d\n", i + 1);
		// Geração da população inicial
		pop = init_pop(parametros);
		// Avalia a população inicial
		evaluate(pop, parametros, grafo);
		gen_actual = 1;
		// Como ainda não existe, escolhe-se como melhor solução a primeira da população que é válida (poderia ser outra qualquer)
		best_run = escolha_inicial(pop, parametros);
		// Encontra-se a melhor solução dentro de toda a população
		best_run = get_best(pop, parametros, best_run);
		// Reserva espaço para os pais da população seguinte
		parents = malloc(sizeof(chrom) * parametros.popsize);
		// Caso não consiga fazer a alocação, envia aviso e termina o programa
		if (parents == NULL)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		// Ciclo de optimização
		while (gen_actual <= parametros.numGenerations)
		{
			// Torneio para encontrar os progenitores (ficam armazenados no vector parents)
			tournament_geral(pop, parametros, parents);
			// Aplica os operadores genéticos aos pais (os descendentes ficam armazenados na estrutura pop)
			genetic_operators(parents, parametros, pop, subopt);
			// Avalia a nova população (a dos filhos)
			evaluate(pop, parametros, grafo);
			// Actualiza a melhor solução encontrada
			best_run = get_best(pop, parametros, best_run);
			gen_actual++;
		}		

		// Escreve resultados da repetição que terminou
	//	printf("\nRepeticao %d:", i);
	//	write_best(best_run, parametros);

		mbf += best_run.fitness;
		if (i == 0 || best_run.fitness < best_ever.fitness) { 
				best_ever = best_run;
				flag = best_ever.fitness;
				
		}
	
		// (Mera curiosidade)Verificar quantos vezes se repetiu uma solução que seja a melhor.
		if (best_run.fitness == best_ever.fitness) { 
			if (i == 0)
				auxf = flag;

			if (auxf == flag)
				fitnessIgual++;
			else{
				if (auxf > flag) {
					fitnessIgual = 1;
					auxf = flag;
				}
			}
				
		}
		//Liberta a memória usada
		free(parents);
		free(pop);
	}
	// Escreve resultados globais
	printf("\n\nNumero de reps. com um fitness/custo igual: %d", fitnessIgual);
	printf("\nMBF: %f\n", mbf / i);
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, parametros);
	printf("\n\n");
}

void fazPesquisaHibrido() {
	char		nome_fich[100];
	int			reps, vert, arest, i, flag, auxf, gen_actual, fitnessIgual = 0;
	int			subopt;
	pchrom      pop = NULL, parents = NULL;
	chrom       best_run, best_ever;
	int*		grafo;
	float		mbf = 0.0;
	info		parametros;

	system("cls");
	printf("Nome do ficheiro: ");
	scanf(" %99[^\n]", nome_fich);
	printf("\nRepeticoes: ");
	scanf("%d", &reps);
	system("cls");

	do {
		printf("Escolha o operador genetico de mutacao:\n");
		printf("1 - Troca\n2 - Insercao\n3 - Inversao\nOpcao: ");
		scanf("%d", &subopt);
	} while (subopt <= 0 || subopt >= 4);

	init_rand();
	grafo = init_dados(nome_fich, &vert, &arest);
	parametros = preenchePara(vert, arest);
	info_expAtual(nome_fich, reps, 0);

	for (i = 0; i < reps; i++)
	{
		printf("Repeticao %d\n", i + 1);
		// Geração da população inicial
		pop = init_pop(parametros);
		// Avalia a população inicial
		evaluate(pop, parametros, grafo);
		gen_actual = 1;
		//Trepa-Colinas que vai analisar a população inicial
		//trepa_colinasH(pop, parametros, grafo);
		// Como ainda não existe, escolhe-se como melhor solução a primeira da população que é válida (poderia ser outra qualquer)
		best_run = escolha_inicial(pop, parametros);
		// Encontra-se a melhor solução dentro de toda a população
		best_run = get_best(pop, parametros, best_run);
		// Reserva espaço para os pais da população seguinte
		parents = malloc(sizeof(chrom) * parametros.popsize);
		// Caso não consiga fazer a alocação, envia aviso e termina o programa
		if (parents == NULL)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		// Ciclo de optimização
		while (gen_actual <= parametros.numGenerations)
		{
			// Torneio binário para encontrar os progenitores (ficam armazenados no vector parents)
			tournament_geral(pop, parametros, parents);
			// Aplica os operadores genéticos aos pais (os descendentes ficam armazenados na estrutura pop)
			genetic_operators(parents, parametros, pop, subopt);
			// Avalia a nova população (a dos filhos)
			evaluate(pop, parametros, grafo);
			// Actualiza a melhor solução encontrada
			best_run = get_best(pop, parametros, best_run);
			gen_actual++;
		}
	    trepa_colinasH(pop, parametros, grafo); // Trepa-Colinas que vai analisar no final das gerações

		// Escreve resultados da repetição que terminou
		printf("\nRepeticao %d:", i);
		mbf += best_run.fitness;
		if (i == 0 || best_run.fitness < best_ever.fitness) {
			best_ever = best_run;
			flag = best_ever.fitness;

		}

		// Verificar quantos vezes se repetiu uma solução que seja a melhor.
		if (best_run.fitness == best_ever.fitness) {
			if (i == 0)
				auxf = flag;

			if (auxf == flag)
				fitnessIgual++;
			else {
				if (auxf > flag) {
					fitnessIgual = 1;
					auxf = flag;
				}
			}

		}
		//Liberta a memória usada
		free(parents);
		free(pop);
	}
	// Escreve resultados globais
	printf("\n\nNumero de reps. cujo o melhor fitness/custo surge : %d", fitnessIgual);
	printf("\nMBF: %f\n", mbf / i);
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, parametros);
	printf("\n\n");
}