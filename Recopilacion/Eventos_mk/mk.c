#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>

static void keyboard (event_handler* ev);
static void mouse (event_handler* ev);


event_handler* mouse_handler_new(int fd) {
	return event_handler_new(fd, mouse);
}

static void mouse (event_handler* ev) {
	
		int fifo= -1;
	
////////////////////////////////////////////////////////////////
// 		APERTURA DEL ARCHIVO ESPECIAL PARA SOLO ESCRITURA     //
////////////////////////////////////////////////////////////////

	fifo= open("events.log", O_WRONLY);
	
////////////////////////////////////////////////////////////////
// 			   	LECTURA DEL EVENTO DEL TECLADO           	  //
////////////////////////////////////////////////////////////////
	
	//printf("ev->fd %d\n",ev->fd);
	char bufer[1024];
	int p= read (ev->fd, bufer, 1024);
	//printf("bytes: %d\n", p);
	
	if (p <0)
		reactor_quit(ev->r);
	printf("bytes leidos del descriptor mouse: %d\n", p);
	
// Representación de los bytes del evento
	int i;
	
	printf("EVENTO: \n");
	for(i=0; i<p; ++i) {
		printf("%x", bufer[i]);
	}
	puts("");
	
	

// Un evento de raton corresponde con 7 bytes
	
////////////////////////////////////////////////////////////////
// 	    DECLARACIÓN DE LA ESTRUCTURA A INTRODUCIR EN FIFO     //
////////////////////////////////////////////////////////////////
		
		
	typedef struct raton{
	char tipo;
	char buf[1024];
	//memset(buf, '\0', sizeof(buf));
	//buf=bufer;
	} movimiento;
	
	
	movimiento evento; //= {'R', bufer};
	evento.tipo = 'R';
	//strcpy(evento.buf, bufer); // No funciona
	//strncpy(evento.buf, bufer, p);

i=0;

	for(i=0; i<p; ++i) {
	//printf("%x", bufer[i]);
	
	evento.buf[i] = bufer[i];
	}
	//puts("");
	
	
	
	
	
	
	
	
	
////////////////////////////////////////////////////////////////
// 	    		   ESCRITURA DEL EVENTO EN FIFO               //
////////////////////////////////////////////////////////////////

	printf("evento.tipo: %c\n", evento.tipo);
	//printf("evento.buf[0]: %x\n", evento.buf[6]);

	printf("EVENTO EN evento.buf: \n");

i= 0;
	
	for(i=0; i<p; ++i) {
		printf("%x", evento.buf[i]);
	}
	
puts("");

	int a;
	a= write(fifo, &evento, sizeof(evento));
	printf("a: %d\n", a);
	
////////////////////////////////////////////////////////////////
// 		 CIERRE DEL ARCHIVO ESPECIAL PARA SOLO ESCRITURA      //
////////////////////////////////////////////////////////////////
	
	close(fifo);
	
	
////////////////////////////////////////////////////////////////
// 	       		    LECTURA DEL EVENTO EN FIFO                //
////////////////////////////////////////////////////////////////

/*
 	//int fifo2= -1;
	
	char bufer2[1025];
	
	fifo= open("events.log", O_RDONLY);
	
	int s;
	
	s= read (fifo, bufer2, sizeof(bufer2));
	
	printf("s: %d\n", s);
	
	
	movimiento* m= (movimiento*) bufer2;
	
	printf("bufer2[0]: %c\n", bufer2[0]);
	// printf("%c\n", bufer2[1]);
	
	printf ("lectura fifo \n");
	printf("%c\n", m->tipo);
	printf("%x\n", m->buf[0]);
	printf("%x\n", m->buf[2]);
	//puts("");
	i= 0;
	for(i=0; i<p; ++i) {
	printf("%x", m->buf[i]);
	}
	puts("");
	
//i= 1;

/*
	for(i=1; i<p; ++i) {
	printf("%x", m->buf[i]);
	}
	puts("");
	* */
	

	
	/*
	printf("%x", m->buf[0]);
	printf("%x", m->buf[1]);
	printf("%x", m->buf[2]);
	printf("%x", m->buf[3]);
	printf("%x", m->buf[4]);
	printf("%x", m->buf[5]);
	printf("%x\n", m->buf[6]);
	
	
		close(fifo);
	
	
*/

}

event_handler* keyboard_handler_new() {
	return event_handler_new(0, keyboard);
}

static void keyboard (event_handler* ev) {
	

	int fifo= -1;
	
////////////////////////////////////////////////////////////////
// 		APERTURA DEL ARCHIVO ESPECIAL PARA SOLO ESCRITURA     //
////////////////////////////////////////////////////////////////

	fifo= open("events.log", O_WRONLY);
	
////////////////////////////////////////////////////////////////
// 			   	LECTURA DEL EVENTO DEL TECLADO           	  //
////////////////////////////////////////////////////////////////
	
	char bufer[1];
	int s= read (ev->fd, bufer, sizeof(bufer));
	if (s <0 || bufer[0]=='q')
		reactor_quit(ev->r);
	
	// Una tecla pulsada corresponde con un byte
	printf("pulsando %c\n", bufer[0]);
	
////////////////////////////////////////////////////////////////
// 	    DECLARACIÓN DE LA ESTRUCTURA A INTRODUCIR EN FIFO     //
////////////////////////////////////////////////////////////////
		

	typedef struct teclado{
	char tipo;
	char buf[1];
	} tecla;


	
	tecla pulsacion;
	pulsacion.tipo = 'T';
	//strcpy(pulsacion.buf, bufer);
	pulsacion.buf[0]=bufer[0];
	

	printf("TIPO \n");
	printf("%c\n", pulsacion.tipo);
	printf("BUFER \n");
	printf("%c\n", pulsacion.buf[0]);
	


////////////////////////////////////////////////////////////////
// 	    		   ESCRITURA DEL EVENTO EN FIFO               //
////////////////////////////////////////////////////////////////
	int d;
	d= write(fifo, &pulsacion, sizeof(pulsacion));
	printf("d: %d\n", d);
	
	
	close(fifo);
////////////////////////////////////////////////////////////////
// 	       		    LECTURA DEL EVENTO EN FIFO                //
////////////////////////////////////////////////////////////////

	
 /*
	
	char bufer2[2];
	
	fifo= open("events.log", O_RDONLY);
	
	read (fifo, bufer2, sizeof(bufer2));
	
	
	tecla* t= (tecla*) bufer2;
	
	printf("%c\n", bufer2[0]);
	printf("%c\n", bufer2[1]);
	
	
	
	printf ("lectura fifo \n");
	printf("%c\n", t->tipo);
	printf("%c\n", t->buf[0]);
	
	
	close(fifo);
	* */
}

	

int main () {
	/*
	struct {
		
		char tipo;
		char buf;
		buf= evento_teclado;
		
	} teclado;
	
	teclado= {'T', buf};
	
	int fifo= -1;
	*/
	
	int fd1 = open("/dev/hidraw1", O_RDONLY);
	printf("fd1 %d\n",fd1);
	/*
	fifo= open("events.log", O_WRONLY);
	*/
	
	
	
	void* state= console_set_raw_mode(0);
	reactor *r= reactor_new();
	reactor_add(r, keyboard_handler_new());
	reactor_add(r, mouse_handler_new(fd1));
	
//	write(fifo, &teclado, sizeof(teclado));
	
	reactor_run(r);
	console_restore(0,  state);
	
	
	
	
	
	
	
	
	
	return 0;
	
}
