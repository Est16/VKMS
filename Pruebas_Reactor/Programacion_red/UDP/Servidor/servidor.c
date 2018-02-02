#include <reactor/reactor.h>
#include <reactor/socket_handler.h>
#include <stdio.h>

void handler(event_handler* ev)
{
	endpoint* ep = (endpoint*)ev;
	char buf[128];
	size_t len = endpoint_recv(ep, buf, sizeof(buf));
	endpoint_send(ep, buf, len);
}

int main () {
	reactor* r = reactor_new();
	reactor_add(r, (event_handler*)udp_endpoint_new("8888", handler));
	reactor_run(r);
	reactor_destroy(r);
	return 0;
}
