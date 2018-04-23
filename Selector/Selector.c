#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>


typedef struct {
	char tipo;
	int x, y, r;
} eyetracking;

eyetracking* e;



	static void eye (event_handler* ev);

	event_handler* eye_handler_new(int fifo) {
		return event_handler_new(fifo, eye);
	}
	

	char function () {
	
	char salida;
	char pantalla= 'I'; // Posicion de la pantalla secundaria
	
	// x3 es el límite derecho de la pantalla principal
	// x1 es el límite izquierdo de la pantalla principal
	
	int x3= 30;
	int x1= 30;
	
	switch (pantalla) {
		
		case 'D':
		
			if (e->x<x3) {
				// Se envía "B" al cliente cuando se mira a la derecha
				// y existe una pantalla a la derecha
				salida= 'B';
			} 
			else {
				salida = 'A';
			}
			break;
			break;
	
		case 'I': // default:
		
			if (e->x>x1) {
				
				// Se envía "B" al cliente cuando se mira a la izquierda
				// y existe una pantalla a la izquierda
				salida = 'B';
			}
			else {
				salida= 'A';
			}	
			break;
			break;
	}
	printf("salida: %c\n", salida);
	return salida;
}
 

	static void eye (event_handler* ev) {
		
		int s;
		char buf[512];
		s= read (ev->fd, buf, sizeof(buf));
		printf("s: %d\n", s);
		
		if (s <0)
		reactor_quit(ev->r);
		
			
		if (buf[0] == 'P') {
			e = (eyetracking*)buf;
			printf("(%d, %d)/%d\n", e->x, e->y, e->r);
			char salida= function();	
			int fifo2= -1;	
			fifo2 = open("select", O_RDWR);
			printf("fifo2: %d\n",fifo2);
			
			int p;
			char buf1[1];
			buf1[0]= salida;
			p= write (fifo2, buf1, sizeof(buf1));
			printf("p: %d\n", p);
			printf("Salida introducida: %c\n", salida);
				
				if (p <0)
				reactor_quit(ev->r);
			
			close(fifo2);
			
		}

	}


	int main () {
		
		// O_RDWR
		
		
		int fifo1= -1;	
		fifo1 = open("eye_events", O_RDONLY);
		printf("fifo1: %d\n",fifo1);
		void* state= console_set_raw_mode(0);
		reactor *r= reactor_new();
		reactor_add(r, eye_handler_new(fifo1));
		reactor_run(r);
		console_restore(0,  state);
		close(fifo1);
		
		return 0;
		
	}






