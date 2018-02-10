#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>

int main() {
	int teclado = open("/dev/hidraw0", O_RDONLY);
	int raton = open("/dev/hidraw1", O_RDONLY);

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(teclado, &fds);
	FD_SET(raton, &fds);

if (teclado<0)
	printf("Error al abrir los archivos\n");
	

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
		for(int i=0; i<n; ++i) {
			printf("%x", buf[i]);
		}
	
		puts("");
		if (n<0)
			perror("Al leer");
	}
	return 0;
}
