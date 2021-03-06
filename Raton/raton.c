#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>


static void mouse (event_handler* ev);

event_handler* mouse_handler_new(int fd) {
	return event_handler_new(fd, mouse);
}

static void mouse (event_handler* ev) {
	printf("ev->fd %d\n",ev->fd);
	char buf[1024];
	int p= read (ev->fd, buf, 1024);
	printf("bytes: %d\n", p);
	if (p <0)
		reactor_quit(ev->r);
		
	for(int i=0; i<p; ++i) {
		printf("%x", buf[i]);
	}
	puts("");
}

int main () {
	
	int fd1 = open("/dev/hidraw1", O_RDONLY);
		printf("fd1 %d\n",fd1);
	
	void* state= console_set_raw_mode(0);
	reactor *r= reactor_new();
	reactor_add(r, mouse_handler_new(fd1));
	reactor_run(r);
	console_restore(0,  state);
	return 0;
	
}


/*
 * #include <reactor/reactor.h>
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


event_handler* keyboard_handler_new() {
		printf("6\n");
	return event_handler_new(0, keyboard);
}

static void keyboard (event_handler* ev) {
	char buf[1];
		printf("ev->fd %d\n",ev->fd);
	if (read (ev->fd, buf, 1) <0 || buf[0]=='q')
		reactor_quit(ev->r);
	printf("pulsando %c\n", buf[0]);
}



event_handler* mouse_handler_new(int fd) {
	return event_handler_new(fd, mouse);
}

static void mouse (event_handler* ev) {
	printf("ev->fd %d\n",ev->fd);
	char buf[1024];
	int p= read (ev->fd, buf, 1024);
	printf("bytes: %d\n", p);
	if (p <0)
		reactor_quit(ev->r);
		
	for(int i=0; i<p; ++i) {
		printf("%x", buf[i]);
	}
	puts("");
	
}

int main () {
	
	
	int teclado = open("/dev/hidraw1", O_RDONLY); // no es cero
	int raton = open("/dev/hidraw0", O_RDONLY);

	printf("teclado %d\n", teclado);
	printf("raton %d\n", raton);
	
		int fd1 = open("/dev/hidraw0", O_RDONLY);
		printf("fd1 %d\n",fd1);
		
		//int fd1=3; //no funciona!
	
	void* state= console_set_raw_mode(0);
	reactor *r1= reactor_new();
		printf("1\n");
	reactor *r2= reactor_new();
		printf("2\n");
	reactor_add(r1, keyboard_handler_new());
		printf("3\n");
	reactor_add(r2, mouse_handler_new(fd1));
		printf("4\n");
	reactor_run(r1,r2);
		printf("5\n");
	//reactor_run(r2);
	console_restore(0,  state);
	return 0;
	
}
*/
