#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>



static void keyboard (event_handler* ev);

event_handler* keyboard_handler_new(int fd) {
	event_handler* ev= event_handler_new(fd, keyboard);
	//printf("Descriptor1: %d\n", ev->fd);
	return ev;
}


static void keyboard (event_handler* ev) {
	//printf("Descriptor3: %d\n", ev->fd);
	char buf[1024];
	int n= read(ev->fd, buf, 1024);
	
	printf("%d ", n);
	for(int i=0; i<n; ++i) {
		printf("%x", buf[i]);
		}
	puts("");
		
	if (n <0)
		reactor_quit(ev->r);	
}

int main () {
	
	int fd = open("/dev/hidraw0", O_RDONLY);
	//printf("Descriptor2: %d\n", fd);
	
	if (fd==-1)
	printf ("Error al abrir el archivo\n");
	
	void* state= console_set_raw_mode(0);
	reactor *r= reactor_new();
	reactor_add(r, keyboard_handler_new(fd));
	reactor_run(r);
	console_restore(0,  state);
	return 0;
	
}
	

	
