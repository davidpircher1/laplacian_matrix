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
				// 40% �anca na generovanie 0 pre ve�kos� matice > 3
				if (rand() % 10 < 4)  // 0 a� 3 (teda 40% �anca na 0)
					matrix[i][x] = 0;
				else
					matrix[i][x] = rand() % 9 + 1;  // ��sla 1 a� 9
			}
			else {
				matrix[i][x] = rand() % 10;  // Pre men�ie matice od 0 do 9
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
	fflush(stdout); // Vypr�zdni buffer a zobraz� v�stup
}

void free_matrix(int size, int** m) {
	// Uvo�nenie pam�te
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
		// Pre matice 4x4 a v��ie (Laplaceova expanzia)
		int** new_matrix = NULL;
		int sign = 1; // Ur�ujeme znamienko pre ka�d� element v matici

		for (int i = 0; i < size; i++) {
			// Pre submatice generujeme nov� riadok/st�pec bez "current_best_start" riadku alebo st�pca
			create_New_Matrix(size, m, &new_matrix, best_start, is_row, i);

			// Rekurz�vne volanie determinantu pre submatice
			int sub_det = determinant(size - 1, new_matrix, best_start, is_row);

			// Ur�enie znamienka pod�a poz�cie (i + j) - striedame znamenka
			sign = ((i + best_start) % 2 == 0) ? 1 : -1;

			// V�pis skr�ten�, bez detailov
			printf("Cofactor = %d., sub_det = %d, %s%d * %d = %d\n\n",
				i, sub_det,
				(sign == 1) ? "+" : "-",
				(is_row ? m[best_start][i] : m[i][best_start]),
				sub_det,
				sign * (is_row ? m[best_start][i] : m[i][best_start]) * sub_det);

			// Pripo��tanie alebo odpo��tanie hodnoty k celkov�mu determinantu
			det += sign * (is_row ? m[best_start][i] : m[i][best_start]) * sub_det;

			// Uvo�nenie pam�te pre new_matrix po pou�it�
			free_matrix(size - 1, new_matrix);
		}
	}

	return det;
}

// Funkcia na n�jdenie riadku alebo st�pca s najviac nulami
void find_best_start(int size, int** m, int* best_start, int* is_row) {
	if (size < 4 && size > 2) {
		printf("Pouzi sarusovo pravidlo.");
		return;
	}

	int best_s = -1;
	int max_zeros = -1;
	int is_rw = -1;

	// Pre ka�d� riadok po��tame po�et n�l
	for (int i = 0; i < size; i++) {
		int zero_count_row = 0;
		int zero_count_col = 0;

		// Po��tanie n�l v riadku
		for (int j = 0; j < size; j++) {
			if (m[i][j] == 0) {
				zero_count_row++;
			}
		}

		// Po��tanie n�l v st�pci
		for (int j = 0; j < size; j++) {
			if (m[j][i] == 0) {
				zero_count_col++;
			}
		}

		// Ak m� tento riadok alebo st�pec viac n�l, aktualizujeme najlep�� index
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
	// Alok�cia novej matice (size-1) x (size-1)
	int** new_matrix = (int**)malloc((size - 1) * sizeof(int*));
	for (int i = 0; i < size - 1; i++) {
		new_matrix[i] = (int*)malloc((size - 1) * sizeof(int));
	}

	if (is_row) { //podla riadku
		int new_i = 0;
		for (int i = 0; i < size; i++) {
			if (i == best_start) continue;  // Presko��me vybran� riadok

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
			if (i == best_start) continue;  // Presko��me vybran� riadok

			int new_j = 0;
			for (int j = 0; j < size; j++) {
				if (j == skip) continue; //preskocime postupne vybrany stlpec
				new_matrix[new_j][new_i] = m[j][i];
				new_j++;
			}
			new_i++;
		}
	}

	// V�pis v�slednej kofaktorovej matice
	print_matrix(size - 1, new_matrix);

	*new_m = new_matrix;

}