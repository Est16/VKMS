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
#include <core_expt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <parse_conf.h>

#include "eyelink_2_error.h"
#include "eyelink_2_types.h"
#include <libGaze/libGaze_types.h>
#include <libGaze/interfaces/libGaze_eye_tracker_module.h>


char* parse_event_filter(t_event_filter *ef);
char* parse_sample_data(t_sample_data *sd);

char* parse_event_filter(t_event_filter *ef){
	if(ef !=NULL){
		short int is_first=1;
		char *s;
		s = (char*)malloc(sizeof(char)*256);
		strcpy(s,"");
		if(ef->left){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"LEFT");
		}

		if(ef->right){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"RIGHT");
		}
		if(ef->fixation){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"FIXATION");
		}
		if(ef->fixupdate){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"FIXUPDATE");
		}
		if(ef->saccade){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"SACCADE");
		}
		if(ef->blink){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"BLINK");
		}
		if(ef->message){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"MESSAGE");
		}
		if(ef->button){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"BUTTON");
		}
		if(ef->input){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"INPUT");
		}
		_DEBUG_("Eyeink2:\ts: %s\n",s);
		if(!strcmp(s,"")){
			return NULL;
		}
		return s;

	}else{
		return NULL;
	}
}

char* parse_event_data(t_event_data *ed){
	if(ed !=NULL){
		short int is_first=1;
		char *s;
		s = (char*)malloc(sizeof(char)*256);
		strcpy(s,"");
		if(ed->gaze){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"GAZE");
		}

		if(ed->gazeres){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"GAZERES");
		}
		if(ed->href){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"HREF");
		}
		if(ed->area){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"AREA");
		}
		if(ed->velocity){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"VELOCITY");
		}
		if(ed->status){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"STATUS");
		}
		if(ed->fixavg){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"FIXAVG");
		}
		if(ed->nostart){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"NOSTART");
		}
		_DEBUG_("Eyeink2:\ts: %s\n",s);
		if(!strcmp(s,"")){
			return NULL;
		}
		return s;

	}else{
		return NULL;
	}
}



char* parse_sample_data(t_sample_data *sd){
		if(sd !=NULL){
		char *s;
		short int is_first=1;
		s = (char*)malloc(sizeof(char)*256);
		strcpy(s,"");
		if(sd->left){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"LEFT");
		}
		if(sd->right){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"RIGHT");
		}
		if(sd->gaze){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"GAZE");
		}
		if(sd->gazeres){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"GAZERES");
		}
		if(sd->href){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"HREF");
		}
		if(sd->pupil){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"PUPIL");
		}
		if(sd->area){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"AREA");
		}

		if(sd->status){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"STATUS");
		}

		if(sd->button){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"BUTTON");
		}
		if(sd->input){
			if(!is_first){
				strcat(s,",");
			}else{
				is_first=0;
				strcpy(s,"");
			}
			strcat(s,"INPUT");
		}

		if(!strcmp(s,"")){
			return NULL;
		}
		return s;
	}else{
		return NULL;
	}
}



t_eye_tracker_config* parse_config_file2(char *filename){
	//char s[128];
	//printf("parse_config_file2: %s\n",filename);
	t_eye_tracker_config *cfg;
	cfg = (t_eye_tracker_config*)malloc(sizeof(t_eye_tracker_config));

	//pcnf_flags_drop(PCNF_FLAG_DEBUG_OUTPUT_VARNAMES);
	pcnf_t *pcnf = pcnf_alloc();

	pcnf_read(pcnf,filename);
	//pcnf_dump(pcnf,stdout);
	char* tmp_s;
	tmp_s = pcnf_sget(pcnf,"EYELINK","IP");
	cfg->et_ip = (char*)malloc((strlen(tmp_s)+1)*sizeof(char));
	strcpy(cfg->et_ip,tmp_s);

	tmp_s =pcnf_sget(pcnf,"EYELINK","NodeName");
	cfg->node_name = (char*)malloc((strlen(tmp_s)+1)*sizeof(char));
	strcpy(cfg->node_name,tmp_s);

	cfg->frequency=pcnf_iget(pcnf,"EYELINK","frequency");
	cfg->fix_threshold=pcnf_fget(pcnf,"EYELINK","fixThreshold");
	cfg->fix_deviation=pcnf_fget(pcnf,"EYELINK","fixDeviation");

	cfg->screen_x=pcnf_iget(pcnf,"EYELINK","screenX");
	cfg->screen_y=pcnf_iget(pcnf,"EYELINK","screenY");
	cfg->screen_w=pcnf_iget(pcnf,"EYELINK","screenWidth");
	cfg->screen_h=pcnf_iget(pcnf,"EYELINK","screenHeight");

	tmp_s = pcnf_sget(pcnf,"EYELINK","calibrationType");
	cfg->calibration_type = (char*)malloc((strlen(tmp_s)+1)*sizeof(char));
	strcpy(cfg->calibration_type,tmp_s);

	cfg->binocular_enabled=pcnf_iget(pcnf,"EYELINK","binocularEnabled");
	cfg->head_subsample_rate=pcnf_iget(pcnf,"EYELINK","headSubsampleRate");
	cfg->heuristic_filter=pcnf_iget(pcnf,"EYELINK","heuristicFilter");

	cfg->simulate_head_camera = pcnf_iget(pcnf,"EYELINK","simulateHeadCamera");
	cfg->simulate_screen_distance = pcnf_iget(pcnf,"EYELINK","simulateScreenDistance");

	cfg->parser_configuration=pcnf_iget(pcnf,"EYELINK","parserConfiguration");


	cfg->edfEnabled = pcnf_iget(pcnf,"EDF_FILE_RECORDING","enable");
	if(cfg->edfEnabled){

		//pcnf_sget(cfg->edfFileName,127,"destinationFile","EDF_FILE_RECORDING");

		t_event_filter *fef;
		fef = (t_event_filter*)malloc(sizeof(t_event_filter));
		fef->left=pcnf_iget(pcnf,"FILE_EVENT_FILTER","left");
		fef->right=pcnf_iget(pcnf,"FILE_EVENT_FILTER","right");
		fef->fixation=pcnf_iget(pcnf,"FILE_EVENT_FILTER","fixation");
		fef->fixupdate=pcnf_iget(pcnf,"FILE_EVENT_FILTER","fixupdate");
		fef->saccade=pcnf_iget(pcnf,"FILE_EVENT_FILTER","saccade");
		fef->blink=pcnf_iget(pcnf,"FILE_EVENT_FILTER","blink");
		fef->message=pcnf_iget(pcnf,"FILE_EVENT_FILTER","message");
		fef->button=pcnf_iget(pcnf,"FILE_EVENT_FILTER","button");
		fef->input=pcnf_iget(pcnf,"FILE_EVENT_FILTER","input");

		cfg->file_event_filter=fef;

		t_event_data  *fed;
		fed = (t_event_data*)malloc(sizeof(t_event_data));
		fed->gaze = pcnf_iget(pcnf,"FILE_EVENT_DATA","gaze");
		fed->gazeres = pcnf_iget(pcnf,"FILE_EVENT_DATA","gazeres");
		fed->href = pcnf_iget(pcnf,"FILE_EVENT_DATA","href");
		fed->area = pcnf_iget(pcnf,"FILE_EVENT_DATA","area");
		fed->velocity = pcnf_iget(pcnf,"FILE_EVENT_DATA","velocity");
		fed->status = pcnf_iget(pcnf,"FILE_EVENT_DATA","status");
		fed->fixavg = pcnf_iget(pcnf,"FILE_EVENT_DATA","fixavg");
		fed->nostart = pcnf_iget(pcnf,"FILE_EVENT_DATA","nostart");

		cfg->file_event_data = fed;


		t_sample_data *fsd;
		fsd = (t_sample_data*)malloc(sizeof(t_sample_data));
		fsd->left= pcnf_iget(pcnf,"FILE_SAMPLE_DATA","left");
		fsd->right=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","right");
		fsd->gaze= pcnf_iget(pcnf,"FILE_SAMPLE_DATA","gaze");
		fsd->gazeres=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","gazeres");
		fsd->href=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","href");
		fsd->pupil=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","pupil");
		fsd->area=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","area");
		fsd->status=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","status");
		fsd->button=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","button");
		fsd->input=pcnf_iget(pcnf,"FILE_SAMPLE_DATA","input");

		cfg->file_sample_data = fsd;
	}else{
		cfg->file_event_filter = NULL;
		cfg->file_sample_data = NULL;
	}

	t_event_filter *lef;
	lef = (t_event_filter*)malloc(sizeof(t_event_filter));
	lef->left=pcnf_iget(pcnf,"LINK_EVENT_FILTER","left");
	lef->right=pcnf_iget(pcnf,"LINK_EVENT_FILTER","right");
	lef->fixation=pcnf_iget(pcnf,"LINK_EVENT_FILTER","fixation");
	lef->fixupdate=pcnf_iget(pcnf,"LINK_EVENT_FILTER","fixupdate");
	lef->saccade=pcnf_iget(pcnf,"LINK_EVENT_FILTER","saccade");
	lef->blink=pcnf_iget(pcnf,"LINK_EVENT_FILTER","blink");
	lef->message=pcnf_iget(pcnf,"LINK_EVENT_FILTER","message");
	lef->button=pcnf_iget(pcnf,"LINK_EVENT_FILTER","button");
	lef->input=pcnf_iget(pcnf,"LINK_EVENT_FILTER","input");

	cfg->link_event_filter=lef;


	t_event_data  *led;
	led = (t_event_data*)malloc(sizeof(t_event_data));
	led->gaze = pcnf_iget(pcnf,"LINK_EVENT_DATA","gaze");
	led->gazeres = pcnf_iget(pcnf,"LINK_EVENT_DATA","gazeres");
	led->href = pcnf_iget(pcnf,"LINK_EVENT_DATA","href");
	led->area = pcnf_iget(pcnf,"LINK_EVENT_DATA","area");
	led->velocity = pcnf_iget(pcnf,"LINK_EVENT_DATA","velocity");
	led->status = pcnf_iget(pcnf,"LINK_EVENT_DATA","status");
	led->fixavg = pcnf_iget(pcnf,"LINK_EVENT_DATA","fixavg");
	led->nostart = pcnf_iget(pcnf,"LINK_EVENT_DATA","nostart");
	cfg->link_event_data = led;


	t_sample_data *lsd;
	lsd = (t_sample_data*)malloc(sizeof(t_sample_data));
	lsd->left= pcnf_iget(pcnf,"LINK_SAMPLE_DATA","left");
	lsd->right=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","right");
	lsd->gaze= pcnf_iget(pcnf,"LINK_SAMPLE_DATA","gaze");
	lsd->gazeres=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","gazeres");
	lsd->href=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","href");
	lsd->pupil=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","pupil");
	lsd->area=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","area");
	lsd->status=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","status");
	lsd->button=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","button");
	lsd->input=pcnf_iget(pcnf,"LINK_SAMPLE_DATA","input");


	cfg->link_sample_data=lsd;
	pcnf_free(pcnf);
	return cfg;

}


int configure(t_eye_tracker_config *config){
	if(!eyelink_is_connected()){
		return -1;
	}

	/*empties the key queue on the host pc*/
	flush_getkey_queue();

	/*sets up the display coordinates left, top, right, bottom*/

	_DEBUG_("tscreen_pixel_coords = %d %d %d %d\n",config->screen_x, config->screen_y, config->screen_w, config->screen_h);
	eyecmd_printf("screen_pixel_coords = %d %d %d %d",config->screen_x, config->screen_y, config->screen_w, config->screen_h);

	/*sets callibration type*/

	_DEBUG_("calibration_type = %s\n",config->calibration_type);
	eyecmd_printf("calibration_type = %s",config->calibration_type);

	/* SET UP TRACKER CONFIGURATION */
	/* set parser saccade thresholds (conservative settings) */
	  	/* 0 = standard sensitivity */

  	_DEBUG_("select_parser_configuration =%d\n",config->parser_configuration);
  	eyecmd_printf("select_parser_configuration =%d",config->parser_configuration);

	if(config->binocular_enabled){
		_DEBUG_("binocular_enabled = YES\n");
  		eyecmd_printf("binocular_enabled = YES");
	}else{
		_DEBUG_("binocular_enable = NO\n");
  		eyecmd_printf("binocular_enable = NO");

	}


	_DEBUG_("head_subsample_rate = %d\n",config->head_subsample_rate);
	eyecmd_printf("head_subsample_rate = %d",config->head_subsample_rate);


  	_DEBUG_("heuristic_filter = %d\n",config->heuristic_filter);
  	eyecmd_printf("heuristic_filter = %d",config->heuristic_filter);


  	if(config->simulate_head_camera){
  		_DEBUG_("simulate_head_camera = YES\n");
  		eyecmd_printf("simulate_head_camera = YES");
  	}else{
  		_DEBUG_("simulate_head_camera = NO\n");
  		eyecmd_printf("simulate_head_camera = NO");
  	}

	_DEBUG_("simulation_screen_distance = %d\n",config->simulate_screen_distance);
  	eyecmd_printf("simulation_screen_distance = %d",config->simulate_screen_distance);



	//EYELINK I
	///*configuration for eye link 1*/
	//eyecmd_printf("saccade_velocity_threshold = 35");
	///*9500 for cognitive research and 5000 for pursuit*/
	//eyecmd_printf("saccade_acceleration_threshold = 9500");


	char *s;
	s= parse_event_filter(config->file_event_filter);

	/* set EDF file contents */
	/*activates the events on which the tracker should listen to. (log file)*/

	if(s!=NULL){
		_DEBUG_("file_event_filter = %s\n",s);
		eyecmd_printf("file_event_filter = %s",s);
	}else{
		_DEBUG_("file_event_filter = NULL\n");
	}

	s = parse_event_data(config->file_event_data);
	if(s!=NULL){
		_DEBUG_("file_event_data = %s\n",s);
		eyecmd_printf("file_event_data = %s",s);
	}else{
		_DEBUG_("file_event_data = NULL\n");
	}

	s= parse_sample_data(config->file_sample_data);

	/*the data which should be written in the log file)*/

	if(s!=NULL){
		_DEBUG_("file_sample_data  = %s\n",s);
		eyecmd_printf("file_sample_data  = %s",s);
	}else{
		_DEBUG_("file_sample_data = NULL\n");
	}


	s=parse_event_filter(config->link_event_filter);

	/* set link data (used for gaze cursor) */
	/*activates the events on which the tracker should listen to. (link)*/

	if(s!=NULL){

		_DEBUG_("link_event_filter = %s\n",s);
		eyecmd_printf("link_event_filter = %s",s);
	}

	s = parse_event_data(config->link_event_data);
	if(s!=NULL){
		_DEBUG_("link_event_data = %s\n",s);
		eyecmd_printf("link_event_data = %s",s);
	}else{
		_DEBUG_("link_event_data = NULL");
	}

	s=parse_sample_data(config->link_sample_data);

	/*the data which should be written in the data-structure, which will be send over the link)*/

	if(s!=NULL){
		_DEBUG_("link_sample_data  = %s\n",s);
		eyecmd_printf("link_sample_data  = %s",s);
	}

	/* Program button #5 for use in drift correction */

	HOOKFCNS *hooks;
	hooks=(HOOKFCNS*)malloc(sizeof(HOOKFCNS));
	init_my_hooks(hooks);
	setup_graphic_hook_functions(hooks);


	/*
	if(config->edfEnabled){
		eyecmd_printf("open_data_file %s",config->edfFileName);
	}
	*/
	return 0;
}

