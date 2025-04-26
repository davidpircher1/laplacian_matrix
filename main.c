#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "determinant.h"

int main() {
	int **matrix = NULL; //nasa premenna matice
	unsigned int seed = time(0); //seed podla aktualneho casu
	int size = 1; //velkost matice
	int*** test = NULL;
	srand(time(0));

	while (1) {
		printf("Algoritmus pouzity je Laplaceov rozvoj, neodporucam pouzit vacsie matice ako 8x8.\n\n\n");
		if (size == 0)break;
		int best_start = NULL;
		int is_row = -1;
		printf("SEED: %d\n\n", seed);
		printf("Zadaj velkost matice: ");
		scanf("%d", &size);
		printf("\n");

		//generuj maticu
		generate_matrix(size, &matrix);

		//vypis maticu
		printf("Nahodna matica: \n");
		print_matrix(size, matrix);

		//najdenie najlepsieho zaciatku pre 4x4 a viac
		find_best_start(size, matrix, &best_start, &is_row);
		if (is_row == 1)
			printf("Zacni laplaceov rozvoj v riadku cislo %d\n\n", best_start);
		else if (is_row == 0)
			printf("Zacni laplaceov rozvoj v stlpci cislo %d\n\n", best_start);

		//vysledok
		printf("Finalny determinant pre maticu %dx%d: %d\n\n", size, size, determinant(size, matrix, best_start, is_row));

		//uvolnenie pamate
		free_matrix(size, matrix);
	}
	printf("Dakujem za pouzivanie programu!");
	return 0;
}
