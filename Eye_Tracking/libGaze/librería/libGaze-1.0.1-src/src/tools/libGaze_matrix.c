#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>

//right handed coordinate system rotation
//see http://www.euclideanspace.com/maths/algebra/matrix/orthogonal/rotation/index.htm
int lg_tools_matrix_rotationXYZ(gsl_matrix* matrix,char axis, double degree){

	if (axis == 'x'){
		gsl_matrix_set(matrix,0,0,1.0);
		gsl_matrix_set(matrix,0,1,0.0);
		gsl_matrix_set(matrix,0,2,0.0);

		gsl_matrix_set(matrix,1,0,0.0);
		gsl_matrix_set(matrix,1,1,cos(degree));
		gsl_matrix_set(matrix,1,2,-sin(degree));

		gsl_matrix_set(matrix,2,0,0.0);
		gsl_matrix_set(matrix,2,1,sin(degree));
		gsl_matrix_set(matrix,2,2,cos(degree));
	}else if(axis == 'y'){
		gsl_matrix_set(matrix,0,0,cos(degree));
		gsl_matrix_set(matrix,0,1,0.0);
		gsl_matrix_set(matrix,0,2,sin(degree));

		gsl_matrix_set(matrix,1,0,0.0);
		gsl_matrix_set(matrix,1,1,1.0);
		gsl_matrix_set(matrix,1,2,0.0);

		gsl_matrix_set(matrix,2,0,-sin(degree));
		gsl_matrix_set(matrix,2,1,0.0);
		gsl_matrix_set(matrix,2,2,cos(degree));

	}else if(axis == 'z'){
		gsl_matrix_set(matrix,0,0,cos(degree));
		gsl_matrix_set(matrix,0,1,-sin(degree));
		gsl_matrix_set(matrix,0,2,0.0);

		gsl_matrix_set(matrix,1,0,sin(degree));
		gsl_matrix_set(matrix,1,1,cos(degree));
		gsl_matrix_set(matrix,1,2,0.0);

		gsl_matrix_set(matrix,2,0,0.0);
		gsl_matrix_set(matrix,2,1,0.0);
		gsl_matrix_set(matrix,2,2,1.0);

	}
	return 1;
}

#define EPSILON 0.000001


void lg_tools_decompose_rotation_matix(double *matrix, double *eulerX, double *eulerY, double *eulerZ){
	double qw,qx,qy,qz;
	double trace = matrix[0]+matrix[4]+matrix[8]+1.0;
	if(trace> 0.00000001){
		double s= 0.5 / sqrt(trace);
		qw = 0.25 /s;
		qx = (matrix[7]-matrix[5])*s;
		qy = (matrix[2]-matrix[6])*s;
		qz = (matrix[3]-matrix[1])*s;
	}else{
		if(matrix[0]>matrix[4]&&matrix[0]>matrix[8]){
			double s= 2.0*sqrt(1.0 +matrix[0]-matrix[4]-matrix[8]);
			qw = (matrix[5]-matrix[7])*s;
			qx = 0.25 *s;
			qy = (matrix[1]+matrix[3])*s;
			qz = (matrix[2]+matrix[6])*s;
		}else if(matrix[4]>matrix[8]){
			double s= 2.0*sqrt(1.0 +matrix[4]-matrix[0]-matrix[8]);
			qw = (matrix[2]-matrix[6])*s;
			qx = (matrix[1]+matrix[3])*s;
			qy = 0.25 *s;
			qz = (matrix[5]+matrix[7])*s;
		}else{
			double s= 2.0*sqrt(1.0 +matrix[8]-matrix[0]-matrix[4]);
			qw = (matrix[1]-matrix[3])*s;
			qx = (matrix[2]+matrix[6])*s;
			qy = (matrix[5]+matrix[7])*s;
			qz = 0.25 *s;
		}
	}

	//double heading2, attitude2,bank2;
	*eulerY = atan2(2*qy*qw-2*qx*qz,1-2*qy*qy-2*qz*qz);
	*eulerZ = asin(2*qx*qy+2*qz*qw);
	*eulerX = atan2(2*qx*qw-2*qy*qz,1-2*qx*qx-2*qz*qz);
}



