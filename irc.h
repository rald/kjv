#ifndef IRC_H
#define IRC_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "common.h"


void raw(int conn,char *fmt, ...);

void privmsg(int conn,char *dst,char *fmt, ...);


#ifdef IRC_IMPLEMENTATION

void raw(int conn,char *fmt, ...) {
	char p[STRING_MAX];

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(p, STRING_MAX, fmt, ap);
	va_end(ap);

	printf("<< %s", p);
	write(conn,p,strlen(p));
	sleep(2);
}



void privmsg(int conn,char *dst,char *fmt, ...) {
	char p[STRING_MAX];
	char b[256],c[STRING_MAX];
	size_t i,j;

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(p, STRING_MAX, fmt, ap);
	va_end(ap);

	printf("<< %s", p);

	j=0;
	while(p[j]) {
		i=0;
		while(i<255 && p[j]) {
			b[i]=p[j];
			i++;
			j++;
		}
		b[i]='\0';				
		sprintf(c,"PRIVMSG %s :%s\r\n",dst,b);
		write(conn,c,strlen(c));
		sleep(2);
	}

}



#endif



#endif
