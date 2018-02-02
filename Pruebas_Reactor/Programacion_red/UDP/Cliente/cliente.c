#include <reactor/reactor.h>
#include <reactor/socket_handler.h>


int main () {
	endpoint* ep = udp_connector_new("localhost", "8888", NULL);
	char buf[]= "Prueba";
	endpoint_send(ep, buf, sizeof(buf));
	endpoint_destroy(ep);
	return 0;
}
