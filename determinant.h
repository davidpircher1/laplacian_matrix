#pragma once
#ifndef determinant_h
#define determinant_h

void generate_matrix(int size, int*** m);
void print_matrix(int size, int** m);
void free_matrix(int size, int** m);
int determinant(int size, int** m, int best_start, int is_row);
void find_best_start(int size, int** m, int* best_start, int* is_row);
void create_New_Matrix(int size, int** m, int*** new_m, int best_start, int is_row, int skip);

#endif