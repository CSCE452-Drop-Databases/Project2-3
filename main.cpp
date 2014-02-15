/*
    File: main.cpp

    Authors: Wesley Tang
            Department of Computer Science
            Texas A&M University
		

*/


#include "paintarm.h"

int main () {
	PaintArm _p = PaintArm();

	// T MATRICIES FOR EACH LINK
	(_p.get_T_Matrix(0, 0))->print(std::cout);
		// Base to first joint
	(_p.get_T_Matrix(1, 1))->print(std::cout);
		// First joint to second
	(_p.get_T_Matrix(2, 2))->print(std::cout);
		// Second joint to third
	(_p.get_T_Matrix(3, 3))->print(std::cout);
		// Third joint to brush head

	// FORWARD KINEMATICS T MATRICIES 
	(_p.get_T_Matrix(0, 0))->print(std::cout);
		// Base to first joint
	(_p.get_T_Matrix(0, 1))->print(std::cout);
		// Base to second joint
	(_p.get_T_Matrix(0, 2))->print(std::cout);
		// Base to third joint
	(_p.get_T_Matrix(0, 3))->print(std::cout);
		// Base to brush head

	/*
		The (x, y) coordinates can be extracted from
		Forward Kinematics T matricies. Given the base
		at (0, 0), the brush head can be found at 
		(_p.get_T_Matrix(0, 3))[0][3] = x and
		(_p.get_T_Matrix(0, 3))[1][3] = y.
	*/
}



