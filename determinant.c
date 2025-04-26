#include <stdio.h>
#include <stdlib.h>
#include "determinant.h"

void generate_matrix(int size, int*** m) {

	//alokacia riadkov
	int** matrix = (int**)malloc(size * sizeof(int*));
	//alokacia prvkov v riadku
	for (int i = 0; i < size; i++) {
		matrix[i] = (int*)malloc(size * sizeof(int));
	}

	//naplnenie matice
	for (int i = 0; i < size; i++) {
		for (int x = 0; x < size; x++) {
			if (size > 3) {
				// 40% šanca na generovanie 0 pre ve¾kos matice > 3
				if (rand() % 10 < 4)  // 0 až 3 (teda 40% šanca na 0)
					matrix[i][x] = 0;
				else
					matrix[i][x] = rand() % 9 + 1;  // Èísla 1 až 9
			}
			else {
				matrix[i][x] = rand() % 10;  // Pre menšie matice od 0 do 9
			}
		}
	}

	//vratenie matice do main
	*m = matrix;
}

void print_matrix(int size, int** m) {
	//vypis matice
	for (int i = 0; i < size; i++) {
		for (int x = 0; x < size; x++) {
			printf("%d ", (m[i][x]));
		}
		printf("\n");
	}
	fflush(stdout); // Vyprázdni buffer a zobrazí výstup
}

void free_matrix(int size, int** m) {
	// Uvo¾nenie pamäte
	for (int i = 0; i < size; i++) {
		free(m[i]);
	}
	free(m);
}

int determinant(int size, int** m, int best_start, int is_row) {
	int det = 0;
	if (size == 2)
	{
		//2x2 matica determinant
		det = (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
		return det;
	}
	else if (size == 3) {
		//3x3 matica determinant
		det = (m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]);
		det -= (m[0][1] * m[1][0] * m[2][2] + m[0][0] * m[1][2] * m[2][1] + m[0][2] * m[1][1] * m[2][0]);
		return det;
	}
	else if (size >= 4) {
		// Pre matice 4x4 a väèšie (Laplaceova expanzia)
		int** new_matrix = NULL;
		int sign = 1; // Urèujeme znamienko pre každý element v matici

		for (int i = 0; i < size; i++) {
			// Pre submatice generujeme nový riadok/ståpec bez "current_best_start" riadku alebo ståpca
			create_New_Matrix(size, m, &new_matrix, best_start, is_row, i);

			// Rekurzívne volanie determinantu pre submatice
			int sub_det = determinant(size - 1, new_matrix, best_start, is_row);

			// Urèenie znamienka pod¾a pozície (i + j) - striedame znamenka
			sign = ((i + best_start) % 2 == 0) ? 1 : -1;

			// Výpis skrátený, bez detailov
			printf("Cofactor = %d., sub_det = %d, %s%d * %d = %d\n\n",
				i, sub_det,
				(sign == 1) ? "+" : "-",
				(is_row ? m[best_start][i] : m[i][best_start]),
				sub_det,
				sign * (is_row ? m[best_start][i] : m[i][best_start]) * sub_det);

			// Pripoèítanie alebo odpoèítanie hodnoty k celkovému determinantu
			det += sign * (is_row ? m[best_start][i] : m[i][best_start]) * sub_det;

			// Uvo¾nenie pamäte pre new_matrix po použití
			free_matrix(size - 1, new_matrix);
		}
	}

	return det;
}

// Funkcia na nájdenie riadku alebo ståpca s najviac nulami
void find_best_start(int size, int** m, int* best_start, int* is_row) {
	if (size < 4 && size > 2) {
		printf("Pouzi sarusovo pravidlo.");
		return;
	}

	int best_s = -1;
	int max_zeros = -1;
	int is_rw = -1;

	// Pre každý riadok poèítame poèet núl
	for (int i = 0; i < size; i++) {
		int zero_count_row = 0;
		int zero_count_col = 0;

		// Poèítanie núl v riadku
		for (int j = 0; j < size; j++) {
			if (m[i][j] == 0) {
				zero_count_row++;
			}
		}

		// Poèítanie núl v ståpci
		for (int j = 0; j < size; j++) {
			if (m[j][i] == 0) {
				zero_count_col++;
			}
		}

		// Ak má tento riadok alebo ståpec viac núl, aktualizujeme najlepší index
		int max_zeros_in_row_or_col = (zero_count_row > zero_count_col) ? zero_count_row : zero_count_col;
		if (max_zeros_in_row_or_col > max_zeros) {
			max_zeros = max_zeros_in_row_or_col;
			is_rw = (zero_count_row > zero_count_col) ? 1 : 0;
			best_s = i;
		}
	}
	*best_start = best_s;
	*is_row = is_rw;

}


void create_New_Matrix(int size, int** m, int*** new_m, int best_start, int is_row, int skip) {
	// Alokácia novej matice (size-1) x (size-1)
	int** new_matrix = (int**)malloc((size - 1) * sizeof(int*));
	for (int i = 0; i < size - 1; i++) {
		new_matrix[i] = (int*)malloc((size - 1) * sizeof(int));
	}

	if (is_row) { //podla riadku
		int new_i = 0;
		for (int i = 0; i < size; i++) {
			if (i == best_start) continue;  // Preskoèíme vybraný riadok

			int new_j = 0;
			for (int j = 0; j < size; j++) {
				if (j == skip) continue; //preskocime postupne vybrany stlpec
				new_matrix[new_i][new_j] = m[i][j];
				new_j++;
			}
			new_i++;
		}
	}
	else { //podla stlpcu
		int new_i = 0;
		for (int i = 0; i < size; i++) {
			if (i == best_start) continue;  // Preskoèíme vybraný riadok

			int new_j = 0;
			for (int j = 0; j < size; j++) {
				if (j == skip) continue; //preskocime postupne vybrany stlpec
				new_matrix[new_j][new_i] = m[j][i];
				new_j++;
			}
			new_i++;
		}
	}

	// Výpis výslednej kofaktorovej matice
	print_matrix(size - 1, new_matrix);

	*new_m = new_matrix;

}