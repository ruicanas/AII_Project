void tournament_geral(pchrom pop, info d, pchrom parents);

void genetic_operators(pchrom parents, info d, pchrom offspring, int opt);

void order_crossover(pchrom parents, info d, pchrom offspring);

void cx_order(int p1[], int p2[], int d1[], int d2[], info d);

void mutacao_troca(pchrom offspring, info d);

void mutacao_insercao(pchrom offspring, info d);

void mutacao_inversao(pchrom offspring, info d);
