//#include <reactor/reactor.h>
//#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>

#include <stdlib.h>
	
	
int main() {
	
	int fifo= -1;
	
	
	typedef struct raton{
	char tipo;
	char buf[1024];
	} movimiento;
	
	char bufer2[1025];
	while (1) {
	fifo= open("events.log", O_RDONLY);
	
	int s;
	
	s= read (fifo, bufer2, sizeof(bufer2));
	
	printf("s: %d\n", s);
	
	
	movimiento* m= (movimiento*) bufer2;

	printf("TIPO \n");
	printf("%c\n", m->tipo);
	
	if (m->tipo=='T') {
		
		printf("BUFER \n");
		printf("%c\n", m->buf[0]);
		
	}
	
	if (m->tipo=='R') {
		
		int i;
		printf("BUFER \n");
			for(i=0; i<7; ++i) {
			printf("%x", m->buf[i]);
			}
		puts("");
		
	}
	
	


	

	

	
		close(fifo);
		
		sleep(5);
	
	}

		
		return 0;
	}
