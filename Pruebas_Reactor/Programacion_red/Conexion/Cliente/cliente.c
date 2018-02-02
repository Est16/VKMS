#include <reactor/reactor.h>
#include <reactor/socket_handler.h>
#include <reactor/event_handler.h>
#include <stdio.h>


void handler(event_handler* ev)
{
	char buf[128];
	int n = event_handler_recv(ev, buf, sizeof(buf));
	buf[n] = '\0';
	printf("Got: %s\n", buf);
}

int main () 
{
	reactor* r = reactor_new();
	connector* c = connector_new("localhost", "10000", handler);
	reactor_add(r, (event_handler*)c);
	connector c_aux = *c;
	
	void producer(event_handler* ev)
	{
		static int i;
		char buf[128];
		snprintf(buf, 128, "Prueba %d", i++);
		connector_send(&c_aux, buf, strlen(buf));
	}
	
	reactor_add(r, (event_handler*)periodic_handler_new(1000, producer));
	reactor_run(r);
	return 0;
}
