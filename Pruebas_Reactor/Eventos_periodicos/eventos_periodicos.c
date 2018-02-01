#include <reactor/reactor.h>
#include <reactor/periodic_handler.h>
#include <reactor/exception.h>
#include <stdio.h>

void handler(event_handler* ev) 
{
	// static int i=0;
	puts("Tick");
	//if (++i >= 5)
		// throw Exception(0,"");
}

int main () 
{
	reactor* r= reactor_new();
	reactor_add(r, (event_handler*)periodic_handler_new(1000, handler)); //100
	reactor_run(r);
	return 0;
}
