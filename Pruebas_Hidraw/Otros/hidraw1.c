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
	return event_handler_new(fd, keyboard);
}


static void keyboard (event_handler* ev) {
	char buf[1];
	if (read (ev->fd, buf, 1) <0 || buf[0]=='q')
		reactor_quit(ev->r);
	printf("pulsando %c\n", buf[0]);
}

int main () {
	
	int fd = open("/dev/hidraw0", O_RDONLY|O_NONBLOCK);
	
	if (fd==-1)
	printf ("Error al abrir el archivo\n");
	
	void* state= console_set_raw_mode(0);
	reactor *r= reactor_new();
	reactor_add(r, keyboard_handler_new(fd));
	reactor_run(r);
	console_restore(0,  state);
	return 0;
	
}
	
	
	
	
	
	
	
	
	
	// Open: funcion para convertir la ruta en un descriptor de fichero
	//int teclado = open("/dev/hidraw0", O_RDONLY);
	
	/*Devuelve el descriptor o "-1" si  ha ocurrido un error
	 * 
	 * El flags "0_RDONLY" permite que la apertura sea solamente para 
	 * la lectura */
	
			//int raton = open("/dev/hidraw1", O_RDONLY);
	
	//printf("%d\n", fd);
//printf ("%d", teclado);

	//int fd = teclado;
	
	//if (fd==-1) 
	//{
	//	printf ("Error al abrir el archivo\n");
		
	//} else 
	//{
		
	//	char buf[1024];
		/* Read: Lee hasta los 1024 bytes del fichero cuyo descriptor es fd 
		 * y los guarda en el buffer "buf" */
	//	int n = read(fd, buf, 1024);
		/* Devuelve el numero de bytes leidos. Si la lectura es erronea devuelve
		 * un "-1" */
		 
	//	 if (n==-1)
	//	 {
	//		 printf("Error al leer el archivo\n");
	//	 } else 
	//	 {
			// printf("%d", n);
	//		for(int i=0; i<n; ++i)
	//		printf("%02x", buf[i]);
	//	 }
		 
	//}
	
	

	
	
	
	
	
	
	/*

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(teclado, &fds);
	FD_SET(raton, &fds);

	for(;;) {
		fd_set copia = fds;
		int n = select(raton+1, &copia, NULL, NULL, NULL);
		
		int fd = -1;

		if (FD_ISSET(teclado, &copia))
			fd = teclado;

		if (FD_ISSET(raton, &copia))
			fd = raton;

		char buf[1024];
		n = read(fd, buf, 1024);

		printf("%d ", n);
		for(int i=0; i<n; ++i)
			printf("%02x", buf[i]);
		puts("");
		if (n<0)
			perror("Al leer");
	}
	*/
	


/*
 * #include <reactor/reactor.h>
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
*/
