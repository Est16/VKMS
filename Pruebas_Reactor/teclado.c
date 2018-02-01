#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>


static void keyboard (event_handler* ev);

event_handler* keyboard_handler_new() {
	return event_handler_new(0, keyboard);
}

static void keyboard (event_handler* ev) {
	char buf[1];
	if (read (ev->fd, buf, 1) <0 || buf[0]=='q')
		reactor_quit(ev->r);
	printf("pulsando %c\n", buf[0]);
}

int main () {
	
	void* state= console_set_raw_mode(0);
	reactor *r= reactor_new();
	reactor_add(r, keyboard_handler_new());
	reactor_run(r);
	console_restore(0,  state);
	return 0;
	
}
