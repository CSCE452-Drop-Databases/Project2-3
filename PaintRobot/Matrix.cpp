/*
File: matrix.cpp

Authors: Colton Williams, Charles Kernstock, Chris Murray, Paul Gaughan, Wesley Tang
Department of Computer Science
Texas A&M University


*/

#include "stdafx.h"

//#include "Matrix.h"

Matrix::Matrix(int r, int c) {
	if (r <= 0 || c <= 0) {
		printf("ERROR :  Matrix Constructor :: Invalid Dimensions, default %ux%u \n", DEFAULT_MATRIX_SIZE, DEFAULT_MATRIX_SIZE);
		row = DEFAULT_MATRIX_SIZE;
		col = DEFAULT_MATRIX_SIZE;
	}
	else {
		row = r;
		col = c;
	}
	matrix = new double*[row];
	for (int i = 0; i < row; ++i) {
		matrix[i] = new double[col];
	}
	for (int i = 0; (i < row && i < col); ++i) {
		matrix[i][i] = 1;
	}
}

Matrix::~Matrix() {
	for (int i = 0; i < row; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;
}


Matrix* Matrix::multiply(Matrix* mat) {
	if (col != mat->row) {
		printf("ERROR :  Matrix Multiply :: Invalid Dimensions \n");
		return NULL;
	}
	Matrix* product = new Matrix(row, col);
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			product->matrix[i][j] = 0;
		}
	}
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			for (int k = 0; k < row; ++k) {
				product->matrix[i][j] += (matrix[i][k] * mat->matrix[k][j]);
			}
		}
	}
	return product;
}

void Matrix::assign_element(int r, int c, double val) {
	if (r > row || c > col || r < 0 || c < 0) {
		printf("ERROR :  Matrix Element Assignment :: Out of Bounds, row or col\n");
		return;
	}
	matrix[r][c] = val;
}


void Matrix::assign_rotation(Matrix* mat) {
	if (mat->row < (row - 1) || mat->col < (col - 1)) {
		printf("ERROR :  Matrix Rotation Assignment :: Invalid Input Rotation Matrix\n");
		return;
	}
	for (int i = 0; i < (row - 1); ++i) {
		for (int j = 0; j < (col - 1); ++j) {
			matrix[i][j] = mat->matrix[i][j];
		}
	}
}

void Matrix::assign_translation(Matrix* mat) {
	if (mat->row < (row - 1)) {
		printf("ERROR :  Matrix Translation Assignment :: Invalid Input Translation Matrix\n");
		return;
	}
	for (int i = 0; i < (row - 1); ++i) {
		matrix[i][(col - 1)] = mat->matrix[i][(mat->col - 1)];
	}
}

void Matrix::fill_bottom_row() {
	for (int j = 0; j < (col - 1); ++j) {
		matrix[(row - 1)][j] = MATRIX_VALUE_ZERO;
	}
	matrix[(row - 1)][(col - 1)] = MATRIX_VALUE_ONE;
}


double Matrix::get_elem(int row, int col) {
	return matrix[row][col];
}

void Matrix::print(std::ostream& os) {
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < col; ++j) {
			os << matrix[i][j] << ' ';
		}
		os << '\n';
	}
	os << '\n';
}



