#ifndef IRC_H
#define IRC_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "common.h"


void raw(int conn,char *fmt, ...);

void privmsg(int conn,char *dst,char *fmt, ...);



void DieWithUserMessage(const char *msg, const char *detail);

void DieWithSystemMessage(const char *msg);

int Irc_Connect(const char *host, const char *service);


#ifdef IRC_IMPLEMENTATION

void DieWithUserMessage(const char *msg, const char *detail) {
   fputs(msg, stderr);
   fputs(": ", stderr);
   fputs(detail, stderr);
   fputc('\n', stderr);
   exit(1);
}

void DieWithSystemMessage(const char *msg) {
   perror(msg);
   exit(1);
}

int Irc_Connect(const char *host, const char *service) {
   
   // Tell the system what kind(s) of address info we want
   struct addrinfo addrCriteria;                   // Criteria for address match
   memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
   addrCriteria.ai_family = AF_UNSPEC;             // v4 or v6 is OK
   addrCriteria.ai_socktype = SOCK_STREAM;         // Only streaming sockets
   addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

   // Get address(es)
   struct addrinfo *servAddr; // Holder for returned list of server addrs
   
   int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);
   if (rtnVal != 0) {
      DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
   }
   
   int sock = -1;
   for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
      // Create a reliable, stream socket using TCP
      sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
      if (sock < 0) {
         continue;  // Socket creation failed; try next address
      }
   
      // Establish the connection to the echo server
      if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) {
         break;     // Socket connection succeeded; break and return socket
      }
      
      close(sock); // Socket connection failed; try next address
      sock = -1;
   }

   freeaddrinfo(servAddr); // Free addrinfo allocated in getaddrinfo()
   return sock;
}



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
