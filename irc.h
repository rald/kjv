#ifndef IRC_H
#define IRC_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>


void raw(int conn,char *fmt, ...);



#ifdef IRC_IMPLEMENTATION



void raw(int conn,char *fmt, ...) {
	char sbuf[512];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(sbuf, 512, fmt, ap);
	va_end(ap);
	printf("<< %s", sbuf);
	write(conn, sbuf, strlen(sbuf));
	sleep(2);
}



#endif



#endif
