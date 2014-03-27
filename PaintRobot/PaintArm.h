/*
    File: paintarm.h

    Authors: Colton Williams, Charles Kernstock, Chris Murray, Paul Gaughan, Wesley Tang
            Department of Computer Science
            Texas A&M University
		

*/

class PaintArm {
	private:
		Matrix* matT_Z_rot(double theta_in_deg, double offset_x, double offset_y, double offset_z);
		Matrix* matT_base_to_joint_n(int n);
	protected:
		std::vector<Matrix*> _T_Matrices;
		std::vector<Matrix*> base_to_n;
		int num_of_joints;
	public:
		PaintArm();
		~PaintArm();
		
		double deg1;
		double deg2;

		//Returns the Transformation matrix if successful
		//Returns the Identity matrix if there was an error
		Matrix* get_T_Matrix(int start_index, int end_index);

		//Returns the angle in degress that the specified joint is rotated relative to the base
		double PaintArm::get_angle(int joint_index);

		//Returns the current x and y coordinates of the specified joint
		Point getJointCoords(int joint_index);

		//Returns the current x and y coordinates of the end effector
		Point getEndEffectorCoords();

		//Rotates the arm at the specified joint by the angle specified in degrees
		void rotate(int joint_index, double rotation_in_deg);

		//Translates the arm at the specified joint by the given x and y values
		void translate(int joint_index, double dx, double dy);

		//Transforms the PaintArm such that the end effector lands on the specified values
		//Returns 0 if successful, 1 if point was not in reachable workspace
		int PaintArm::calc_Inverse_Kinematics(double xpos, double ypos);
};




