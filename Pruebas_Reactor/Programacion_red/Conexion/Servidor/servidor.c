#include <reactor/reactor.h>
#include <reactor/socket_handler.h>

void handle_echo(event_handler* ev)
{
	endpoint* ep = (endpoint*)ev;
	char buf[128];
	int n = endpoint_recv(ep, buf, sizeof(buf));
	endpoint_send(ep, buf, n);
}

int main ()
{
	reactor* r = reactor_new();
	reactor_add(r, (event_handler*)acceptor_new ("10000", handle_echo));
	reactor_run(r);
	return 0;
}
