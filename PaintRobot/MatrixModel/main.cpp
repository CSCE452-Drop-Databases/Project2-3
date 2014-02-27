/*
    File: main.cpp

    Authors: Colton Williams, Charles Kernstock, Chris Murray, Paul Gaughan, Wesley Tang
            Department of Computer Science
            Texas A&M University
		

*/


#include "paintarm.h"

int main() {
	PaintArm _p = PaintArm();

	std::cout << "Invidiaul T Matricies\n";
	// T MATRICIES FOR EACH LINK
	(_p.get_T_Matrix(0, 0))->print(std::cout);
	// Base to first joint
	(_p.get_T_Matrix(1, 1))->print(std::cout);
	// First joint to second
	(_p.get_T_Matrix(2, 2))->print(std::cout);
	// Second joint to third
	(_p.get_T_Matrix(3, 3))->print(std::cout);
	// Third joint to brush head

	std::cout << "Forward Kinematrics\n";
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
	for (int i = 0; i < 8; ++i) {
		std::cout << "ROTATING JOINT 1 BY 45\n";
		_p.rotate(0, 45);

		(_p.get_T_Matrix(0, 0))->print(std::cout);
		(_p.get_T_Matrix(0, 1))->print(std::cout);
		(_p.get_T_Matrix(0, 2))->print(std::cout);
		(_p.get_T_Matrix(0, 3))->print(std::cout);
	}*/
	std::cout << "ROTATING JOINT 1 BY -90\n";
	_p.rotate(0, 60);

	(_p.get_T_Matrix(0, 0))->print(std::cout);
	(_p.get_T_Matrix(0, 1))->print(std::cout);
	(_p.get_T_Matrix(0, 2))->print(std::cout);
	(_p.get_T_Matrix(0, 3))->print(std::cout);

	/*
	std::cout << "TRANSLATING BASE BY 2\n";
	_p.translate(0, 2, 0);

	(_p.get_T_Matrix(0, 0))->print(std::cout);
	(_p.get_T_Matrix(0, 1))->print(std::cout);
	(_p.get_T_Matrix(0, 2))->print(std::cout);
	(_p.get_T_Matrix(0, 3))->print(std::cout);
	*/
	

	/*
		The (x, y) coordinates can be extracted from
		Forward Kinematics T matricies. Given the base
		at (0, 0), the brush head can be found at 
		(_p.get_T_Matrix(0, 3))[0][3] = x and
		(_p.get_T_Matrix(0, 3))[1][3] = y.

		If you want to rotate a joint, simply call the
		rotate function as demo'd above.
		Note that "rest" position is horizonal along
		positive X-axis but we give it a static rotate
		of -90 to put it into vertical position.

		If you want to translate the base, simply call
		the translate function as demo'd above.
		Note that the base starts off at (0, 0). 
	*/
}



