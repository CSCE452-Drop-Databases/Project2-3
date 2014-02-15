/*
    File: paintarm.cpp

    Authors: Wesley Tang
            Department of Computer Science
            Texas A&M University
		

*/

#include "paintarm.h"

double deci_round(double in) {
	return round(in * 1000)/1000;
}

PaintArm::PaintArm() {
	Matrix* t_mat_base_to_joint_0 = matT_Z_rot(90, 0, 0, 0);
	_T_Matrices.push_back(t_mat_base_to_joint_0);

	Matrix* t_mat_joint_0_to_joint_1 = matT_Z_rot(-90, LINK_LENGTH_1, 0, 0);
	_T_Matrices.push_back(t_mat_joint_0_to_joint_1);

	Matrix* t_mat_joint_1_to_joint_2 = matT_Z_rot(90, LINK_LENGTH_2, 0, 0);
	_T_Matrices.push_back(t_mat_joint_1_to_joint_2);

	Matrix* t_mat_joint_2_to_joint_3 = matT_Z_rot(0, LINK_LENGTH_3, 0, 0);
	_T_Matrices.push_back(t_mat_joint_2_to_joint_3);

	num_of_joints = _T_Matrices.size();

	for (int i = 0; i <= num_of_joints; ++i) {
		base_to_n.push_back(matT_base_to_joint_n(i));
	}
}

PaintArm::~PaintArm() {
	for (int i = 0; i < _T_Matrices.size(); ++i) {
		delete _T_Matrices[i];
	}
	for (int i = 0; i < base_to_n.size(); ++i) {
		delete base_to_n[i];
	}
}


Matrix* PaintArm::get_T_Matrix(int start_index, int end_index) {
	if (start_index > end_index || end_index >= num_of_joints || start_index < 0) {
		printf("ERROR :  PaintArm get_T_Matrix :: Invalid Index\n");
		return base_to_n[0]; // Identity Matrix
	}
	if (start_index == 0) {
		return base_to_n[end_index+1];
	}
	if (start_index == end_index) {
		return _T_Matrices[start_index];
	}
	printf("ERROR :  PaintArm get_T_Matrix :: NO IMPLEMENTATION FOR GIVEN PARAMETERS, DEAL WITH IT.\n");
	return base_to_n[0]; // Identity Matrix
}

void PaintArm::rotate(int joint_index, double rotation_in_deg) {
	if (joint_index >= num_of_joints) {
		printf("ERROR :  PaintArm rotate :: Out of Bounds, joint_index\n");
		return;
	}
	_T_Matrices[joint_index];
	// THINGS TO DO
	// APPLY ROTATION MATRIX TO _T_Matrices[joint_index]
	// UPDATE ALL MATRICIES IN base_to_n WITH INDEX EQUAL TO OR GREATER THAN joint_index
}

Matrix* PaintArm::matT_Z_rot(double theta_in_deg, double offset_x, double offset_y, double offset_z) {

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

Matrix* PaintArm::matT_base_to_joint_n(int n) {
	Matrix* _t = new Matrix(DEFAULT_MATRIX_SIZE, DEFAULT_MATRIX_SIZE);
	if (n > _T_Matrices.size()) {
		printf("ERROR :  PaintArm matT_base_to_joint_n :: Invalid n\n");
		return _t;
	}
	for (int i = 0; i < n; ++i) {
		_t = _t->multiply(_T_Matrices[i]);
	}
	return _t;
}



