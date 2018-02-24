/****************************************************************************
* 	libGaze
*  A framework to combine off-the-shelf video-based eye tracking systems
*	with motion caputing systems
*
*	Copyright(C) 2008
*	Max Planck Institute for Biological Cybernetics Tuebingen
*	BWFIT
*
*	All rights reserved.
*
*	This file is part of libGaze.
*
*	libGaze is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  libGaze is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with libGaze.  If not, see <http://www.gnu.org/licenses/>.
*
****************************************************************************/


#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>

#include <libGaze.h>


#ifndef M_PI
#define M_PI 3.1415927
#endif

double lg_tools_degrees(double radians){
	//_DEBUG_("degrees: %f\n",radians);
	return (radians/M_PI)*180.0;
}

double lg_tools_radians(double degree){
	//_DEBUG_("radians: degree= %f\n",degree);
	//_DEBUG_("radians(%f)=	%f\n",degree,((degree/180.0)*M_PI));
	return (degree/180.0)*M_PI;
}

double *lg_tools_getYZAnglesFrom3DVector(gsl_vector* vec){
	double xz =sqrt(pow(gsl_vector_get(vec,0),2)+pow(gsl_vector_get(vec,2),2));
	double *ab = (double*)malloc(2*sizeof(double));

	if (gsl_vector_get(vec,2)>=0.0){
		ab[0] = acos(gsl_vector_get(vec,0)/xz);
		ab[0] = -lg_tools_degrees(ab[0]);
	}else{
		ab[0] = (2.0*M_PI)-acos(gsl_vector_get(vec,0)/xz);
		ab[0] = -(lg_tools_degrees(ab[0])-360.0);
	}
	//ab[1] = (M_PI/2.0)-atan(gsl_vector_get(vec,1)/xz);
	ab[1] = atan(gsl_vector_get(vec,1)/xz);
	ab[1] = lg_tools_degrees(ab[1]);
	//_DEBUG_("getXYAnglesFrom3DVector: %f\t%f\n",ab[0],ab[1]);

	return ab;
}

gsl_vector *lg_tools_get3DVectorFromAngles(double alpha, double beta){
			//_DEBUG_("get3DVectorFromAngles: alpha: %f\tbeta: %f\n",alpha,beta);
			double x;
			double y;
			double z;

			y = sin(beta);
			x = cos(alpha)*cos(beta);
			z = -sin(alpha)*cos(beta);



			gsl_vector *vec = gsl_vector_alloc(3);
			gsl_vector_set(vec,0,x);
			gsl_vector_set(vec,1,y);
			gsl_vector_set(vec,2,z);

			return vec;
}




