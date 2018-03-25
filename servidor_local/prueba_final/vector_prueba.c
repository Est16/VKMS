#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main () {
	
	int cont = 0;
	int v1[5], v2[5];
	
	for (int i = 0; i<5; i++) {
		
		v1[i]= cont;
		v2[i]= cont+3;
		
		cont++;
	}
	/*
	for (int p = 0; p<5; p++) {
	printf("%d ",v1[p]);
	}
	puts("");
	for (int s = 0; s<5; s++) {
	printf("%d ",v2[s]);
	}
	puts("");
	* */
	int fifo = -1;

	fifo = open("vectores.log", O_WRONLY|O_CREAT, ~0);
	/*
	struct {
		char type;
        cv::Rect eye;
      } s = {
        'E', eye
      };
      ::write(fifo, &s, sizeof(s));
	*/
	
	//int buf[5];
	
	
	write (fifo, v1, 5*sizeof(int)); //1204
	
	
	
	
	
	
	
	
	return 0;
}
