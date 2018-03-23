#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

static int udp_client_socket(const char* host, const char* service);

int main() {
	int fd = udp_client_socket("192.168.1.117", "80");
	
	if (fd < 0) {
	printf("Error fd");
	}
	
	
	
	for(;;) {
		// char *fgets(char *str, int n, FILE *stream)
		//FILE *stdin;
 	
		//int elem_escritos;
		//stdin = fopen ("texto.yml", "r ");
		
		
		
		
		
		
		char buf[1024];
		//buf[0]= 'a';
		//buf[1]= 'b';
		//buf[2]= 'c';
		fgets(buf, sizeof(buf), stdin);
		//printf("%c", buf[0]);
		
		
		int n = write(fd, buf, strlen(buf));
		//
		//printf("%d", n);
		
		assert(n >= 0);		
		if (n == 0) {
		printf("No se pudo enviar\n");
		}
		if (n == 0) break;

	}
	close(fd);
	return 0;
}

static struct sockaddr_in ip_address(const char* host,
						const char* service,
						const char* proto);
						
static int udp_client_socket(const char* host, const char* service)
{
	struct sockaddr_in sin = ip_address(host, service, "udp");
	struct protoent* pe = getprotobyname("udp");
	assert(pe != NULL);
	int fd = socket(PF_INET, SOCK_DGRAM, pe->p_proto);
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

