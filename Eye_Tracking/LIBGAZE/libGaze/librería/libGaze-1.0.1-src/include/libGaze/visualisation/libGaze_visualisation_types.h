/*
 * libGaze_visualisation_types.h
 *
 *  Created on: Sep 17, 2008
 *      Author: racoon
 */

#ifndef LIBGAZE_VISUALISATION_TYPES_H_
#define LIBGAZE_VISUALISATION_TYPES_H_

#include "../libGaze_input_types.h"
#include "../libGaze_types.h"
/**
 * @struct	t_visualisation_hooks
 * @brief	saihsflsdf
 *
 * jfdlkfjslkjslfkdsjflkfjsflksjfslkfjslkfjslkjijcvocxjvdslk
 */
typedef struct{
	///additional pointer which can link to an additional needed structure
	void *vptr;
	/// erases all content on the screen.
	void (*erase_screen)(void* vptr);
	/** draws all content which should be drawn after the last call of erase_screen on the screen
	(buffer swap)*/
	void (*update_screen)(void* vptr);
	/** draws a calibration target along the ray (t_ray *ray). The 3D position of the drawn target
	is saved under pos*/
	int (*draw_cal_target)(void* vptr, t_ray* ray, double* pos);
	/** draws a Validation result of the Validation process along the ray (t_ray* ray)
	the 3D position of the drawn validation result is saved under double* pos
	The flag vtype specificies the type of the validation data which should be drawn:
	<ul>
	\li
	\li VALID_TARGET = the presented validation target
	\li VALID_LEFT = the calculated gaze ray for the left eye
	\li VALID_RIGTH = the calculated gaze ray for the right eye
	</ul>*/
	void (*draw_validation)(void* vptr,  t_ray* ray, double* pos,int vtype);

	void (*draw_fov)(void* vptr, t_ray* ray1, t_ray* ray2, t_ray* ray3, t_ray* ray4);
	void (*draw_head_orientation_target)(void* vptr,  t_ray* ray, int);
	void (*draw_head_orientation)(void* vptr, t_ray* ray, int);
	/// presents a message to the observer
	void (*show_message)(void* vptr,char* msg);
	/// returns the last input done by the user
	void (*get_input)(void* vptr, t_input* input);
	/// clears the input queue
	void (*clear_input)(void* vptr);


}t_visualisation_hooks;

enum visual_validation_types{
	VALID_TARGET = 0,
	VALID_LEFT,
	VALID_RIGHT
};


#endif /* LIBGAZE_VISUALISATION_TYPES_H_ */
