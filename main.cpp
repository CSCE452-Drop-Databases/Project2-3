/*
    File: main.cpp

    Authors: Wesley Tang
            Department of Computer Science
            Texas A&M University
		

*/

#include <iostream>
#include "matrix.h"

#include <cmath>

double deci_round(double in) {
	return round(in * 1000)/1000;
}

Matrix* matT_Z_rot(double theta_in_deg, double offset_x, double offset_y, double offset_z) {

	double theta_in_rads = (theta_in_deg*DEG_TO_RADS);

	Matrix* _t = new Matrix(DEFAULT_MATRIX_SIZE, DEFAULT_MATRIX_SIZE);

	_t->assign_element(0, 0, deci_round(cos(theta_in_rads)));
	_t->assign_element(0, 1, deci_round(-1*sin(theta_in_rads)));
	_t->assign_element(0, 2, MATRIX_VALUE_ZERO);
	_t->assign_element(0, 3, offset_x);

	_t->assign_element(1, 0, deci_round(sin(theta_in_rads)));
	_t->assign_element(1, 1, deci_round(cos(theta_in_rads)));
	_t->assign_element(1, 2, MATRIX_VALUE_ZERO);
	_t->assign_element(1, 3, offset_y);

	_t->assign_element(2, 0, MATRIX_VALUE_ZERO);
	_t->assign_element(2, 1, MATRIX_VALUE_ZERO);
	_t->assign_element(2, 2, MATRIX_VALUE_ONE);
	_t->assign_element(2, 3, offset_z);

	_t->fill_bottom_row();
	return _t;
}

int main () {

	Matrix* t_mat_base_to_joint_0 = matT_Z_rot(90, 0, 0, 0);
	t_mat_base_to_joint_0->print(std::cout);

	Matrix* t_mat_joint_0_to_joint_1 = matT_Z_rot(-90, LINK_LENGTH_1, 0, 0);
	t_mat_joint_0_to_joint_1->print(std::cout);

	Matrix* t_mat_joint_1_to_joint_2 = matT_Z_rot(90, LINK_LENGTH_2, 0, 0);
	t_mat_joint_1_to_joint_2->print(std::cout);

	Matrix* t_mat_joint_2_to_joint_3 = matT_Z_rot(0, LINK_LENGTH_3, 0, 0);
	t_mat_joint_2_to_joint_3->print(std::cout);

	Matrix* t_mat_base_to_joint_3 = 
		t_mat_base_to_joint_0->multiply(
			t_mat_joint_0_to_joint_1->multiply(
				t_mat_joint_1_to_joint_2->multiply(
					t_mat_joint_2_to_joint_3)));

	t_mat_base_to_joint_3->print(std::cout);

}