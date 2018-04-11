#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>




#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
//#include <string.h>

//#include <stdlib.h>
	










static int tcp_client_socket(const char* host, const char* service);
int main() {
	int fd = tcp_client_socket("192.168.1.38", "1000");
	for(;;) {
	
	int fifo= -1;
	
	
	typedef struct raton{
	char tipo;
	char buf[1024];
	} movimiento;
	
	char bufer2[1025];

	fifo= open("events.log", O_RDONLY);
	
	int s;
	
	s= read (fifo, bufer2, sizeof(bufer2));
	
	printf("s: %d\n", s);
	
	
	movimiento* m= (movimiento*) bufer2;

	printf("TIPO \n");
	printf("%c\n", m->tipo);
	
	close(fifo);
	
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
	
	/*

char prueba[7];

prueba[0]= 'A';
prueba[1]= 'B';
prueba[2]= 'C';
prueba[3]= 'D';
prueba[4]= 'E';
prueba[5]= 'F';
prueba[6]= 'G';

char buff1[1];
char buff2[1];
char buff3[1];
char buff4[1];
char buff5[1];
char buff6[1];
char buff7[1];

buff1[0]= 3;
buff2[0]= 0;
buff3[0]= 0;
buff4[0]= 0;
buff5[0]= 0;
buff6[0]= 0;
buff7[0]= 0;

*/

//printf("prueba: %x\n", prueba[1]);
//printf("prueba: %x %x %x %x %x %x %x %x\n", prueba[0], prueba[1], prueba[2], prueba[3], prueba[4], prueba[5], prueba[6], prueba[7]);
		
		//int n= write(fd, prueba, strlen(prueba));
		/*
		int n = write(fd, buff1, sizeof(buff1));
		write(fd, buff2, sizeof(buff2));
		write(fd, buff3, sizeof(buff3));
		write(fd, buff4, sizeof(buff4));
		write(fd, buff5, sizeof(buff5));
		write(fd, buff6, sizeof(buff6));
	    write(fd, buff7, sizeof(buff7));
	    * */
		
		//int n = write(fd, bufer2, strlen(bufer2));
		
		//int p;
		
		// char vector[1]= m->tipo;
		//char vector1[1] = m->buf[0];
		//char vector2[1] = m->buf[1];
		//char vector3[1] = m->buf[2];
		//char vector4[1] = m->buf[3];
		//char vector5[1] = m->buf[4];
		//char vector6[1] = m->buf[5];
		//char vector7[1] = m->buf[6];
		
		
		
		
		
		
		
		/*
		char vector[1];
		vector[0]= bufer2[0];

		int n1 = write(fd, vector, sizeof(vector));
		//int n2 = write(fd, vector1, sizeof(vector1));
		//int n3 = write(fd, vector2, sizeof(vector2));
		//int n4 = write(fd, vector3, sizeof(vector3));
		//int n5 = write(fd, vector4, sizeof(vector4));
		//int n6 = write(fd, vector5, sizeof(vector5));
		//int n7 = write(fd, vector6, sizeof(vector6));
		//int n8 = write(fd, vector7, sizeof(vector7));
			
			
		assert(n1 >= 0);
		if (n1 == 0) break;
*/


		char vector[1];
		vector[0]= bufer2[0];

		int n1 = write(fd, vector, sizeof(vector));
		
		assert(n1 >= 0);
		if (n1 == 0) break;
		
		if (bufer2[0]=='T') {
			
			char vector1[1];
			vector1[0] = bufer2[1];
			int n2 = write(fd, vector1, sizeof(vector1));
			

			assert(n2 >= 0);
			if (n2 == 0) break;
		}
	
		
		
		if (bufer2[0]=='R') {
			
			char vector1[1];
			char vector2[1];
			char vector3[1];
			char vector4[1];
			char vector5[1];
			char vector6[1];
			char vector7[1];
			
			vector1[0] = bufer2[1]; // Pimera posicion
			vector2[0] = bufer2[2]; // Segunda posicion
			vector3[0] = bufer2[3]; // Segunda posicion
			
			printf("vector1[0]: %x\n", vector1[0]);
			printf("vector2[0]: %x\n", vector2[0]);
			//vector3[0] = bufer2[2];
			vector4[0] = bufer2[4];
			vector5[0] = bufer2[5];
			vector6[0] = bufer2[6];
			vector7[0] = bufer2[7];
			
			
			int n3 = write(fd, vector1, sizeof(vector1));
			int n4 = write(fd, vector2, sizeof(vector2));
			int n5 = write(fd, vector3, sizeof(vector3));
			int n6 = write(fd, vector4, sizeof(vector4));
			int n7 = write(fd, vector5, sizeof(vector5));
			int n8 = write(fd, vector6, sizeof(vector6));
			int n9 = write(fd, vector7, sizeof(vector7));
			
			assert(n3 >= 0);
			if (n3 == 0) break;
			assert(n4 >= 0);
			if (n4 == 0) break;
			assert(n5 >= 0);
			if (n5 == 0) break;
			assert(n6 >= 0);
			if (n6 == 0) break;
			assert(n7 >= 0);
			if (n7 == 0) break;
			assert(n8 >= 0);
			if (n8 == 0) break;
			assert(n9 >= 0);
			if (n9 == 0) break;
			
		}
		

		


		/*		
		assert(n4 >= 0);
		if (n4 == 0) break;
		assert(n5 >= 0);
		if (n5 == 0) break;
		assert(n6 >= 0);
		if (n6 == 0) break;
		assert(n7 >= 0);
		if (n7 == 0) break;
		assert(n8 >= 0);
		if (n8 == 0) break;
		assert(n9 >= 0);
		if (n9 == 0) break;

*/

		sleep(8);

	}
	close(fd);
	return 0;
}

static struct sockaddr_in ip_address(const char* host,
				const char* service,
				const char* proto);
static int tcp_client_socket(const char* host, const char* service)
{
	struct sockaddr_in sin = ip_address(host, service, "tcp");
	struct protoent* pe = getprotobyname("tcp");
	assert(pe != NULL);
	int fd = socket(PF_INET, SOCK_STREAM, pe->p_proto);
	assert (fd >= 0);
	assert (connect(fd, (struct sockaddr*)&sin, sizeof(sin)) >= 0);
	return fd;
}


static struct sockaddr_in ip_address(const char* host,
				const char* service,
				const char* proto)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	struct hostent* he = gethostbyname(host);
	if (he != NULL)
	memcpy(&sin.sin_addr, he->h_addr_list[0], he->h_length);
	else
	sin.sin_addr.s_addr = inet_addr(host);
	struct servent* se = getservbyname(service, proto);
	sin.sin_port = (se != NULL? se->s_port : htons(atoi(service)));
	return sin;
}
