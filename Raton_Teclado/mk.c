#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

static void keyboard (event_handler* ev);
static void mouse (event_handler* ev);


event_handler* mouse_handler_new(int fd) {
	return event_handler_new(fd, mouse);
}

static void mouse (event_handler* ev) {
	//printf("ev->fd %d\n",ev->fd);
	char buf[1024];
	int p= read (ev->fd, buf, 1024);
	//printf("bytes: %d\n", p);
	if (p <0)
		reactor_quit(ev->r);
		
	for(int i=0; i<p; ++i) {
		printf("%x", buf[i]);
	}
	puts("");
}

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
	
	int fd1 = open("/dev/hidraw1", O_RDONLY);
	//printf("fd1 %d\n",fd1);
	
	
	
	
	void* state= console_set_raw_mode(0);
	reactor *r= reactor_new();
	reactor_add(r, keyboard_handler_new());
	reactor_add(r, mouse_handler_new(fd1));
	reactor_run(r);
	console_restore(0,  state);
	return 0;
	
}
