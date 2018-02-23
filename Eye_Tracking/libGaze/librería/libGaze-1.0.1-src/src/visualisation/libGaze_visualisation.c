/*
 * libGaze_visualisation.c
 *
 *  Created on: Sep 17, 2008
 *      Author: racoon
 */

#include <stdlib.h>
#include <visualisation/libGaze_visualisation.h>

t_visualisation_hooks* v_alloc_visualisation_hooks(){
	t_visualisation_hooks * vhooks = (t_visualisation_hooks*)calloc(1,sizeof(t_visualisation_hooks));
	return vhooks;
}


void v_erase_screen(t_visualisation_hooks* vhooks){
	(*(vhooks->erase_screen))(vhooks->vptr);

}
void v_update_screen(t_visualisation_hooks* vhooks){
	(*(vhooks->update_screen))(vhooks->vptr);
}
/*
void v_draw_cal_target(t_visualisation_hooks* vhooks, int x, int y){
	(*(vhooks->draw_cal_target))(vhooks->vptr, x, y);
}
*/

int v_draw_cal_target(t_visualisation_hooks* vhooks, t_ray *ray, double *pos){
	return (*(vhooks->draw_cal_target))(vhooks->vptr, ray, pos);
}

void v_draw_validation(t_visualisation_hooks* vhooks, t_ray *ray,double* pos, int type){
	(*(vhooks->draw_validation))(vhooks->vptr,ray,pos,type);
}
/*
void v_draw_circle(t_visualisation_hooks* vhooks, int x, int y, int* color, int radius){
	(*(vhooks->draw_circle))(vhooks->vptr,x,y,color,radius);
}


void v_draw_rect(t_visualisation_hooks* vhooks, int* x, int* y, int* color){
	(*(vhooks->draw_rect))(vhooks->vptr,x,y,color);

}
*/
void v_draw_fov(t_visualisation_hooks* vhooks, t_ray *ray1,t_ray *ray2,t_ray *ray3,t_ray *ray4){
	(*(vhooks->draw_fov))(vhooks->vptr, ray1,ray2,ray3,ray4);

}
void v_draw_head_orientation_target(t_visualisation_hooks* vhooks, t_ray *ray, int size){
	(*(vhooks->draw_head_orientation_target))(vhooks->vptr,ray,size);
}

void v_draw_head_orientation(t_visualisation_hooks* vhooks,t_ray *ray, int size){
	(*(vhooks->draw_head_orientation))(vhooks->vptr,ray,size);
}
void v_show_message(t_visualisation_hooks* vhooks,char* msg){
	(*(vhooks->show_message))(vhooks->vptr,msg);
}

void v_get_input(t_visualisation_hooks* vhooks,t_input* input){
	(*(vhooks->get_input))(vhooks->vptr, input);
}

void v_clear_input(t_visualisation_hooks* vhooks){
	(*(vhooks->clear_input))(vhooks->vptr);
}
