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
	
	const char* IP;
	
	//int fd = tcp_client_socket(IP, "1000");
	for(;;) {
		
	///////////////////////////////////////////////////////////////
	//			   ENVIO DE EVENTOS DE TECLADO Y RATON		     //
	///////////////////////////////////////////////////////////////
	
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
	
	
	///////////////////////////////////////////////////////////////
	//   OBTENCION DEL ARDUINO AL QUE HAY QUE ENVIAR EVENTOS     //
	///////////////////////////////////////////////////////////////
	
	
	int fifo1= -1;
	char bufer3[1];
	fifo1= open("select", O_RDONLY); // 1 caracter ocupa 1 byte
	
	int r;
	
	r= read (fifo1, bufer3, sizeof(bufer3));
	
	if (bufer3[0]=='A') {
		
		IP= "192.168.1.38";
		
	} else if (bufer3[0]=='B') {
		
		IP= "192.168.1.39";
		
	} 
	
	////////////////////////////////////////////////////////////////////////////
	//   ENVIO DE LOS EVENTOS DE TECLADO Y RATON AL ARDUINO CORRESPONDIENTE   //
	////////////////////////////////////////////////////////////////////////////
	
		int fd = tcp_client_socket(IP, "1000");
		int n = write(fd, bufer2, strlen(bufer2));
		assert(n >= 0);
		if (n == 0) break;

		close(fifo);
		
		sleep(5);

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
