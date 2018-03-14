#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int fifo = -1;
/*
typedef struct {
	char tipo;
	int x, y, r;
} eye;
* */

int main() {
	
	/////////////////////////////////////////////////////////////
	//       OBTENER EL VECTOR ABRIENDO Y LEYENDO FIFO         //
	/////////////////////////////////////////////////////////////
	
	fifo = open("vectores.log", O_RDONLY);
	//while(1) {
		int v[5];
		read(fifo, v, 5*sizeof(int)); //512
		
		for (int p = 0; p<5; p++) {
		printf("%d ",v[p]);
		}
		
		puts("");
		/*
		if (buf[0] == 'E') {
			eye* e = (eye*)buf;
			printf("(%d, %d)/%d\n", e->x, e->y, e->r);
		}
		* */
	//}
	
	/////////////////////////////////////////////////////////////
	//        INTRODUCIR EL VECTOR EN UN ARCHIVO .YML          //
	/////////////////////////////////////////////////////////////

/*
 	FILE *f;
 	
	int v[6], elem_escritos, num;
	f = fopen ("archivo.txt", "wb ");
	//Para escribir los 3 últimos elementos de v (el 2, el 3 y el 4)
	 elem-escritos = fwrite (&v[2], sizeof(int), 3, f );
	//Para escribir el primer elemento de v, valen las 2 instrucciones siguientes
	fwrite (v, sizeof (int), 1, f );
	fwrite (&v[0], sizeof(int), 1, f );
	//Para escribir un entero valen las dos siguientes
	fwrite (&num, sizeof(int), 1, f);
	fwrite (&num, sizeof(num), 1, f);
	
	*/
	
	FILE *f;
 	
	int elem_escritos;
	f = fopen ("archivo.yml", "wb ");
	//elem_escritos = fwrite (&v[0], sizeof(int), 5, f );
	
	for (int s = 0; s<5; s++) {
	fprintf ( f, "%d\n", v[s]); 
	}
	
	
	fclose(f);
	
	return 0;
	
}
