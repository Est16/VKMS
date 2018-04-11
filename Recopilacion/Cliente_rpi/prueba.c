//#include <reactor/reactor.h>
#include <reactor/console.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <string.h>
	
	
int main() {
	
	typedef struct evento{
	char tipo;
	char buf1[1];
	char buf2[7];
	} eventRT;
	
	char bufer2[8];
		
	fifo= open("events.log", O_RDONLY);
	
	int s;
	s= read (fifo, bufer2, sizeof(bufer2));
	printf("s: %d\n", s);
	
	
	eventRT* m= (eventRT*) bufer2;
	
		
		
		
	close(fifo);	
		
		
		
		
		
		
		
		
		
		//fgets(buf, sizeof(buf), stdin);
		fgets(buf1, sizeof(buf1), m->tipo);
		fgets(buf2, sizeof(buf2), m->buf[0]);
		
		
		printf("m->tipo %c", m->tipo);
		int i;
		
		for (i=0; i<s, i++) {
		printf("m->buf[0] %x", m->buf[0]);
		
		}
		
		return 0;
	}
