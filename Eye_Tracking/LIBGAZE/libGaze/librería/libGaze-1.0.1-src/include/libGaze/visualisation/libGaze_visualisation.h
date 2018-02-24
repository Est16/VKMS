/*
 * libGaze_visualisation.h
 *
 *  Created on: Sep 17, 2008
 *      Author: racoon
 */

#ifndef LIBGAZE_VISUALISATION_H_
#define LIBGAZE_VISUALISATION_H_

#include "libGaze_visualisation_types.h"

/**
 * Allocates a new t_visualisation_hooks structure.
 *
 * @return a pointer to the new allocated t_visualisation_hooks structure or NULL on error
 */
t_visualisation_hooks* v_alloc_visualisation_hooks();
/**
 *
 *
 */
void v_erase_screen(t_visualisation_hooks* vhooks);
void v_update_screen(t_visualisation_hooks* vhooks);
int v_draw_cal_target(t_visualisation_hooks* vhooks, t_ray* ray, double *pos);
void v_draw_validation(t_visualisation_hooks* vhooks, t_ray* ray, double *pos,int type);
void v_draw_fov(t_visualisation_hooks* vhooks,t_ray*,t_ray*,t_ray*,t_ray*);
void v_draw_head_orientation_target(t_visualisation_hooks* vhooks, t_ray*,int);
void v_draw_head_orientation(t_visualisation_hooks* vhooks,t_ray*,int);
void v_show_message(t_visualisation_hooks* vhooks,char*);
void v_get_input(t_visualisation_hooks* vhooks,t_input*);
void v_clear_input(t_visualisation_hooks* vhooks);
#endif /* LIBGAZE_VISUALISATION_H_ */
