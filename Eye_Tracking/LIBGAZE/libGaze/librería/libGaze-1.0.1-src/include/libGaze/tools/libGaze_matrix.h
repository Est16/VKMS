#ifndef LIBGAZE_MATRIX_H_
#define LIBGAZE_MATRIX_H_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>


int lg_tools_matrix_rotationXYZ(gsl_matrix* matrix,char axis, double degree);

void lg_tools_decompose_rotation_matix(double *matrix, double *eulerX, double *eulerY, double *eulerZ);

#endif /*LIBGAZE_MATRIX_H_*/
