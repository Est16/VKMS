#ifndef LIBGAZE_INPUTTYPES_H_
#define LIBGAZE_INPUTTYPES_H_


enum lg_input_types{
	LG_INPUT_NONE,
	LG_INPUT_QUIT,
	LG_INPUT_ACCEPT,
	LG_INPUT_ABORT	,	
	LG_INPUT_KEY,	
	LG_INPUT_MESSAGE 	
};

typedef struct{
	enum lg_input_types type;
	int key;
	int mod;
	char *msg;
}t_input;


#endif /*LIBGAZE_INPUTTYPES_H_*/
