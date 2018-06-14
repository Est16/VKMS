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
	

	
////////////////////////////////////////////////////////////////
// 		APERTURA DEL ARCHIVO ESPECIAL PARA SOLO ESCRITURA     //
////////////////////////////////////////////////////////////////

	int fifo= -1;	
	fifo = open("archivo", O_RDWR|O_NONBLOCK);
	printf("fiforaton: %d\n",fifo);
	
////////////////////////////////////////////////////////////////
// 			   	LECTURA DEL EVENTO DEL TECLADO           	  //
////////////////////////////////////////////////////////////////
	
	//printf("ev->fd %d\n",ev->fd);
	unsigned char bufer[1024];
	int p= read (ev->fd, bufer, 1024);
	//printf("bytes: %d\n", p);
	
	if (p <0)
		reactor_quit(ev->r);
		printf("bytes leidos del descriptor mouse: %d\n", p);
	
	/*
// Representación de los bytes del evento
	int i;
	
	printf("EVENTO: \n");
	for(i=0; i<p; ++i) {
		printf("%x", bufer[i]);
	}
	puts("");
	
	*/

// Un evento de raton corresponde con 7 bytes
	
////////////////////////////////////////////////////////////////
// 	    DECLARACIÓN DE LA ESTRUCTURA A INTRODUCIR EN FIFO     //
////////////////////////////////////////////////////////////////
		
		
	typedef struct raton{
	char tipo;
	unsigned char buf[1024];
	//memset(buf, '\0', sizeof(buf));
	//buf=bufer;
	} movimiento;
	
	
	movimiento evento; //= {'R', bufer};
	evento.tipo = 'R';
	//strcpy(evento.buf, bufer); // No funciona
	//strncpy(evento.buf, bufer, p);


	for(int i=0; i<p; ++i) {
	//printf("%x", bufer[i]);
	
	evento.buf[i] = bufer[i];
	}
	//puts("");
	
	
	
	
	
	
	
	
	
////////////////////////////////////////////////////////////////
// 	    		   ESCRITURA DEL EVENTO EN FIFO               //
////////////////////////////////////////////////////////////////

	printf("evento.tipo: %c\n", evento.tipo);
	//printf("evento.buf[0]: %x\n", evento.buf[6]);

	printf("evento.buf: \n");

int t= 0;
	
	for(t=0; t<p; ++t) {
		printf("%x", evento.buf[t]);
		printf(" - ");
	}
	
puts("");

	int a;
	a= write(fifo, &evento, p+1); //sizeof(evento)
	printf("numero de bytes escritos: %d\n", a);
	
////////////////////////////////////////////////////////////////
// 		 CIERRE DEL ARCHIVO ESPECIAL PARA SOLO ESCRITURA      //
////////////////////////////////////////////////////////////////
	
	close(fifo);
	
}



event_handler* keyboard_handler_new() {
	return event_handler_new(0, keyboard);
}

static void keyboard (event_handler* ev) {
	

////////////////////////////////////////////////////////////////
// 		APERTURA DEL ARCHIVO ESPECIAL PARA SOLO ESCRITURA     //
////////////////////////////////////////////////////////////////

	int fifo= -1;	
	fifo = open("archivo", O_RDWR|O_NONBLOCK);
	printf("fifoteclado: %d\n",fifo);

	
////////////////////////////////////////////////////////////////
// 			   	LECTURA DEL EVENTO DEL TECLADO           	  //
////////////////////////////////////////////////////////////////
	

	char bufer[1];
	int s= read (ev->fd, bufer, sizeof(bufer));
	if (s <0 || bufer[0]=='q')
		reactor_quit(ev->r);
		printf("bytes leidos del descriptor keyboard: %d\n", s);
	
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
	printf("numero de bytes escritos: %d\n", d);
	
	
	close(fifo);
	
}

	

int main () {

	
	int fd1 = open("/dev/hidraw1", O_RDONLY);
	printf("fd1 %d\n",fd1);
	
	void* state= console_set_raw_mode(0);
	reactor *r= reactor_new();
	reactor_add(r, keyboard_handler_new());
	reactor_add(r, mouse_handler_new(fd1));
	
	reactor_run(r);
	console_restore(0,  state);
	
	
	return 0;
	
}
