cdef struct t_visualisation_hooks	

cdef extern from "libGaze/visualisation/libGaze_visualisation.h":
	t_visualisation_hooks* v_alloc_visualisation_hooks()
	
cdef extern from "pyGaze_visualisation.h":
	void extractVisualHooks(obj,t_visualisation_hooks* vhooks)
