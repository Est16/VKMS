#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int fifo = -1;

typedef struct {
	char tipo;
	int x, y, r;
} eye;

int main() {
	fifo = open("eye_detect.log", O_RDONLY);
	while(1) {
		char buf[512];
		read(fifo, buf, 512);
		if (buf[0] == 'E') {
			eye* e = (eye*)buf;
			printf("(%d, %d)/%d\n", e->x, e->y, e->r);
		}
	}
}
