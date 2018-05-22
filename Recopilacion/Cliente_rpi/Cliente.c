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
	
	const char* IP1;
	IP1= "127.0.0.1";
	const char* IP2;
	IP2= "127.0.0.1";
	
	// Comprobado con netcat en distintos puertos y una misma IP
	int fd1 = tcp_client_socket(IP1, "1000"); 
	int fd2 = tcp_client_socket(IP2, "999");
	
	for(;;) {

	///////////////////////////////////////////////////////////////
	//			   ENVIO DE EVENTOS DE TECLADO Y RATON		     //
	///////////////////////////////////////////////////////////////
	
	int fifo1= -1;
	char bufer2[1025];
	fifo1= open("/home/esther/Desktop/VKMS/Recopilacion/Eventos_mk/archivo", O_RDONLY);
	int s;
	s= read (fifo1, bufer2, sizeof(bufer2));
	printf("numero de bytes leidos: %d\n", s);

	
	///////////////////////////////////////////////////////////////
	//   OBTENCION DEL ARDUINO AL QUE HAY QUE ENVIAR EVENTOS     //
	///////////////////////////////////////////////////////////////
	
	
	int fifo2= -1;
	int descriptor;
	char bufer3[1];
	fifo2= open("/home/esther/Desktop/VKMS/Recopilacion/Selector/select", O_RDONLY); // 1 caracter ocupa 1 byte
	int r;
	r= read (fifo2, bufer3, sizeof(bufer3));
	
	if (bufer3[0]=='A') {
		
		descriptor= fd1;
		
	} else if (bufer3[0]=='B') {
		
		descriptor=fd2;
	} 

	////////////////////////////////////////////////////////////////////////////
	//   ENVIO DE LOS EVENTOS DE TECLADO Y RATON AL ARDUINO CORRESPONDIENTE   //
	////////////////////////////////////////////////////////////////////////////
	
		int n = write(descriptor, bufer2, strlen(bufer2));
		assert(n >= 0);
		if (n == 0) {
			break;
			close(fifo1);
			close(fifo2);
		}

	}
	close(fd1);
	close(fd2);
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
