#ifndef LIBGAZE_VECTOR_AND_ANGLES_H_
#define LIBGAZE_VECTOR_AND_ANGLES_H_

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

double lg_tools_degrees(double radians);

double lg_tools_radians(double degree);

double *lg_tools_getYZAnglesFrom3DVector(gsl_vector* vec);

gsl_vector *lg_tools_get3DVectorFromAngles(double alpha, double beta);


#endif /*LIBGAZE_VECTOR_AND_ANGLES_H_*/
