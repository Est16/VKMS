#ifndef PYGAZE_VISUALISATION_H_
#define PYGAZE_VISUALISATION_H_

//extern PyObject* vobj;
#include "libGaze/libGaze_types.h"
void extractVisualHooks(PyObject*,t_visualisation_hooks*);

void vhook_erase_screen(void*);
void vhook_update_screen(void*);

//void vhook_draw_cal_target(void*,int, int);
//void vhook_draw_circle(void*,int,int,int*,int);
int vhook_draw_cal_target(void*,t_ray*,double*);
void vhook_draw_validation(void*,t_ray*,double*,int);
void vhook_draw_fov(void*,t_ray*,t_ray*,t_ray*,t_ray*);

//void vhook_draw_rect(void*,int* ,int* ,int*);


void vhook_draw_head_orientation_target(void*,t_ray*,int);
void vhook_draw_head_orientation(void*,t_ray*,int);
void vhook_show_message(void*,char*);
void vhook_get_input(void*,t_input*);
void vhook_clear_input(void*);
#endif /*PYGAZE_VISUALISATION_H_*/
