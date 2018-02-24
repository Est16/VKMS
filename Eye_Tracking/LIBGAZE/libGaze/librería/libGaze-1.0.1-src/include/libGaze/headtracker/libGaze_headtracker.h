#ifndef LIBGAZE_HEADTRACKER_H_
#define LIBGAZE_HEADTRACKER_H_


#include "../libGaze_types.h"
#include "libGaze_headtracker_defines.h"


HEAD_TRACKER* ht_load_module(char*,int, int, enum headtracker_modes);

enum headtracker_return ht_unload_module(HEAD_TRACKER*);

enum headtracker_return ht_get_tracker_frequency(HEAD_TRACKER*,int*);

enum headtracker_return ht_connect_to_head_tracker(HEAD_TRACKER*);

enum headtracker_return ht_disconnect_head_tracker(HEAD_TRACKER*);

enum headtracker_return ht_configure_head_tracker(HEAD_TRACKER*,char*);

enum headtracker_return ht_start_head_tracking(HEAD_TRACKER*);

enum headtracker_return ht_stop_head_tracking(HEAD_TRACKER*);

enum headtracker_return ht_get_tracking_data(HEAD_TRACKER*, int, t_raw_head_data*);

enum headtracker_return ht_is_head_tracker_connected(HEAD_TRACKER*);

enum headtracker_return ht_open_log_file(HEAD_TRACKER*,char*,char*);

enum headtracker_return ht_close_log_file(HEAD_TRACKER*);

enum headtracker_return ht_start_logging(HEAD_TRACKER*);

enum headtracker_return ht_stop_logging(HEAD_TRACKER*);

enum headtracker_return ht_add_log_msg(HEAD_TRACKER*,char*);


#endif /*LIBGAZE_HEADTRACKER_H_*/
