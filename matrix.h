/*
    File: matrix.h

    Authors: Wesley Tang
            Department of Computer Science
            Texas A&M University
		

*/

#pragma once
#include <iostream>
#include <stdio.h>

#include "global_const.h"

class Matrix {
	private:
	protected:
		int row;
		int col;
		double** matrix;

	public:
		Matrix(int r, int c);
		~Matrix();

		Matrix* multiply(Matrix* mat);
		void assign_element(int r, int c, double val);
		void assign_rotation(Matrix* mat);
		void assign_translation(Matrix* mat);
		void fill_bottom_row();

		void print(std::ostream& os);
};