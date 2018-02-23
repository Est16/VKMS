#include <stdlib.h>
#include <Python.h>
#include <libGaze/libGaze_types.h>
#include <libGaze/libGaze_defines.h>
#include "pyGaze_visualisation.h"

//PyObject* vobj = NULL;

void extractVisualHooks(PyObject* obj,t_visualisation_hooks* vhooks){
	//printf("extractVisualHooks: %p\n",obj);
	vhooks->vptr =(void*)obj;
	vhooks->erase_screen = vhook_erase_screen;
	vhooks->update_screen = vhook_update_screen;
	//vhooks->draw_cal_target = vhook_draw_cal_target;
	//vhooks->draw_circle = vhook_draw_circle;
	vhooks->draw_cal_target = vhook_draw_cal_target;
	vhooks->draw_validation = vhook_draw_validation;
	//vhooks->draw_rect = vhook_draw_rect;
	vhooks->draw_fov = vhook_draw_fov;
	vhooks->draw_head_orientation_target = vhook_draw_head_orientation_target;
	vhooks->draw_head_orientation = vhook_draw_head_orientation;
	vhooks->show_message = vhook_show_message;
	vhooks->get_input = vhook_get_input;
	vhooks->clear_input = vhook_clear_input;
}

void vhook_erase_screen(void* vptr){
	PyObject_CallMethod((PyObject*)vptr,"eraseScreen","");
}
void vhook_update_screen(void* vptr){
	PyObject_CallMethod((PyObject*)vptr,"updateScreen","");
}
/*
void vhook_draw_cal_target(void* vptr,int x, int y){
	PyObject_CallMethod((PyObject*)vptr,"drawCalTarget","ii",x,y);
}
void vhook_draw_circle(void* vptr,int x, int y,int* rgba, int radius){
	PyObject_CallMethod((PyObject*)vptr,"drawCircle","ii(iiii)i",x,y,rgba[0],rgba[1],rgba[2],rgba[3],radius);
}
*/
int vhook_draw_cal_target(void* vptr,t_ray* ray, double* pos){
	PyObject* pyGazeModule = PyImport_ImportModule("pyGaze");
	PyObject* pyGazeRay = PyObject_GetAttrString(pyGazeModule, "Ray");
	if (pyGazeRay==NULL){
		printf("could not get Ray\n");
	}



	PyObject* pyreturn =PyObject_CallMethod((PyObject*)vptr,"drawCalTarget","(fff)(fff)",ray->origin[0],ray->origin[1],ray->origin[2],ray->direction[0],ray->direction[1],ray->direction[2]);
	if(PyList_Size(pyreturn)==3){
		pos[0] = PyFloat_AsDouble(PyList_GetItem(pyreturn,0));
		pos[1] = PyFloat_AsDouble(PyList_GetItem(pyreturn,1));
		pos[2] = PyFloat_AsDouble(PyList_GetItem(pyreturn,2));
		return 0;
	}
	return -1;
}
void vhook_draw_validation(void* vptr,t_ray* ray,double* pos,int type){
	PyObject* pyreturn =PyObject_CallMethod((PyObject*)vptr,"drawValidation","(fff)(fff)i",ray->origin[0],ray->origin[1],ray->origin[2],ray->direction[0],ray->direction[1],ray->direction[2],type);
	if(PyList_Size(pyreturn)==3){
		if(pos!=NULL){
			pos[0] = PyFloat_AsDouble(PyList_GetItem(pyreturn,0));
			pos[1] = PyFloat_AsDouble(PyList_GetItem(pyreturn,1));
			pos[2] = PyFloat_AsDouble(PyList_GetItem(pyreturn,2));
		}
		//return 0;

	}
	//return -1;
}
/*
void vhook_draw_rect(void* vptr,int* xl, int* yl,int* rgba){
	PyObject_CallMethod((PyObject*)vptr,"drawRect","((ii)(ii)(ii)(ii))(iiii)",xl[0],yl[0],xl[1],yl[1],xl[2],yl[2],xl[3],yl[3],rgba[0],rgba[1],rgba[2],rgba[3]);
}
*/
void vhook_draw_fov(void* vptr,t_ray* ray1,t_ray* ray2,t_ray* ray3,t_ray* ray4){
	PyObject_CallMethod((PyObject*)vptr,"drawFov","((fff)(fff))((fff)(fff))((fff)(fff))((fff)(fff))",ray1->origin[0],ray1->origin[1],ray1->origin[2],ray1->direction[0],ray1->direction[1],ray1->direction[2],ray2->origin[0],ray2->origin[1],ray2->origin[2],ray2->direction[0],ray2->direction[1],ray2->direction[2],ray3->origin[0],ray3->origin[1],ray3->origin[2],ray3->direction[0],ray3->direction[1],ray3->direction[2],ray4->origin[0],ray4->origin[1],ray4->origin[2],ray4->direction[0],ray4->direction[1],ray4->direction[2]);
}
void vhook_draw_head_orientation_target(void* vptr,t_ray* ray, int r){
	PyObject_CallMethod((PyObject*)vptr,"drawHeadOrientationTarget","(fff)(fff)i",ray->origin[0],ray->origin[1],ray->origin[2],ray->direction[0],ray->direction[1],ray->direction[2],r);
}
void vhook_draw_head_orientation(void* vptr,t_ray* ray, int r){
	PyObject_CallMethod((PyObject*)vptr,"drawHeadOrientation","(fff)(fff)i",ray->origin[0],ray->origin[1],ray->origin[2],ray->direction[0],ray->direction[1],ray->direction[2],r);
}
void vhook_show_message(void* vptr,char* msg){
	PyObject_CallMethod((PyObject*)vptr,"showMessage","s",msg);
}

void vhook_get_input(void* vptr,t_input* input){
	//printf("vhook_get_input\n");
	//printf("extractVisualHooks: %p\n",vptr);
	PyObject* pyinput = PyObject_CallMethod((PyObject*)vptr,"getInput","");
	if (pyinput != NULL){
		//printf("pyinput != NULL\n");
		PyObject* pytype = PyObject_GetAttrString(pyinput,"type");
		int type;
		//PyArg_Parse(pytype,"i",&type);
		type = PyInt_AsLong(pytype);
		//printf("type= %d\n",type);
		input->type = type;
		if (input->type== LG_INPUT_KEY){
			PyObject* pykey = PyObject_GetAttrString(pyinput,"key");
			input->key = PyInt_AsLong(pykey);
			//printf("key= %d\n",input->key);
		}
	}else{
		input->type = LG_INPUT_NONE;
	}
}


void vhook_clear_input(void* vptr){
	PyObject_CallMethod((PyObject*)vptr,"clearInput","");
}
