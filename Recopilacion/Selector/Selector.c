#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

////////////// Media movil de la distancia X de O' a 0'' //////////////

// 				Mirada central: 45,46,47,48,49,50,51
// 				Mirada derecha: 59,60,61,62,63
// 				Mirada izquierda: 40,41,42,43,44

#define x1 44
#define x2 59


typedef struct {
	char tipo;
	int x, y, d;
} eyetracking;

eyetracking* e;

	static void eye (event_handler* ev);

	event_handler* eye_handler_new(int fifo) {
		return event_handler_new(fifo, eye);
	}
	
	
	char function (double mediamov) {
	
	char salida;
	char pantalla= 'I'; // Posicion de la pantalla secundaria
	
	
	switch (pantalla) {
		
		case 'D':
		
			if (mediamov>=x2) {
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
		
			if (mediamov<=x1) {
				
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

	return salida;
}
 
 

	static void eye (event_handler* ev) {
		
		int contador=0;
		int vector[5];
		int valor =0;
		
		while (contador<5) { // los valores de contador van de 0 a 4
		int s;
		char buf[512];
		s= read (ev->fd, buf, sizeof(buf));
		printf("s: %d\n", s);
		
			if (s <0)
			reactor_quit(ev->r);
		
			
			if (buf[0] == 'E') {
				e = (eyetracking*)buf;
				printf("[%d, %d] from (%d, %d)\n", e->d, e->d, e->x, e->y);
				
				if (e->x < 70) { // Se descartan los eventos procedentes del ojo derecho
				vector[contador]= e->x;
				contador++;
				}

				}
		}
		
	/*
	 * El bucle realiza lecturas de eventos hasta que rellena las 5 posiciones
	 * del vector con eventos del ojo (cuyo tipo es "E")
	 * */

	for (int i=0; i<contador+1; i++) {
		
		valor= valor + vector[i];
		
	}

	double MM= (double) valor/5;
	
	printf("Media movil: %f\n", MM);
	
	char salida = function(MM);
	printf("salida: %c\n", salida);

	int fifo2= -1;	
	fifo2 = open("select", O_RDWR);
	printf("fifo2: %d\n",fifo2);

	int p;
	char buf1[1];
	buf1[0]= salida;
	p= write (fifo2, buf1, sizeof(buf1));
	printf("p: %d\n", p);
	
		if (p <0)
		reactor_quit(ev->r);
		
	printf("Salida introducida: %c\n", salida);


	}


	int main () {
		
		// O_RDWR
		
		
		int fifo1= -1;
		//int fifo2= -1;		
		fifo1 = open("/home/esther/Desktop/VKMS/Recopilacion/Eye_tracking/eye_fifo", O_RDONLY);
		printf("fifo1: %d\n",fifo1);
		//fifo2 = open("select", O_RDWR);
		//printf("fifo2: %d\n",fifo2);
		
		
		void* state= console_set_raw_mode(0);
		reactor *r= reactor_new();
		reactor_add(r, eye_handler_new(fifo1));
		reactor_run(r);
		console_restore(0,  state);
		close(fifo1);
		//close(fifo2);
		
		return 0;
		
	}






