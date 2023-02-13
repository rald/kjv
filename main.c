#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>



#define IRC_IMPLEMENTATION 
#include "irc.h"

#define INFO_IMPLEMENTATION 
#include "info.h"

#define LEXER_IMPLEMENTATION 
#include "lexer.h"

#define PARSER_IMPLEMENTATION 
#include "parser.h"

#define CITE_IMPLEMENTATION 
#include "cite.h"

#define INITIAL_BUF_LEN 512



char *nick = "siesto";
char *channel = "#pantasya";
char *host = "irc.dal.net";
char *port = "6667";
char *pass = "paanoanggagawinko"; 



int sock;


static Info **infos=NULL;
static size_t ninfos=0;

static Token **tokens=NULL;
static size_t ntokens=0;

static Cite **cites=NULL;
static size_t ncites=0;





int main(void) {
	char *user, *server, *command, *where, *message, *sep, *target;

	int sl, wordcount;

	char buf[512];
	size_t buflen=512;

	char *msg;

//	struct addrinfo hints, *res;

	srand(time(NULL));

	Info_Load(&infos,&ninfos,"kjv.inf");

/*	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(host, port, &hints, &res);
	conn = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(conn, res->ai_addr, res->ai_addrlen);
*/

	sock=Irc_Connect(host,port);
  if (sock<0) {
  	DieWithUserMessage("Irc_Connect() failed", "unable to connect");
  }


   	
	if(pass) raw(sock,"PASS %s\r\n", pass);
	raw(sock,"NICK %s\r\n", nick);
	raw(sock,"USER %s %s %s :%s\r\n", nick, nick, nick, nick);

	while ((sl=readline(sock,buf,buflen))>=0) {
		printf(">> %s", buf);
		
		if (!strncmp(buf, "PING", 4)) {
			buf[1] = 'O';
			raw(sock,"%s",buf);
		} else if (buf[0] == ':') {
			wordcount = 0;
			user = server = command = where = message = NULL;
			
			user=buf+1; 
			wordcount++;
			if((command=skip(user,' '))) {
				wordcount++; 
				if((where=skip(command,' '))) {
					wordcount++;
					if((message=skip(where,' '))) {
						msg=message[0]==':'?message+1:message;
						wordcount++;
					}
				}
			}

/*
			printf("usr: %s\n",user);	
			printf("cmd: %s\n",command);	
			printf("whr: %s\n",where);	
			printf("msg: %s\n\n",message);	
*/

					
			if (wordcount < 2) continue;
			
			if (!strncmp(command, "001", 3)) {
				raw(sock,"JOIN %s\r\n", channel);
			} else if (!strncmp(command, "433", 3)) {			
			
				raw(sock,"NICK %s%04X\r\n", nick,rand()%0xFFFF);
			
				raw(sock,"PRIVMSG %s: NicServ@services.dal.net ghost %s %s\r\n",channel,nick,pass);										
			} else if (!strncmp(command, "PRIVMSG", 7) || !strncmp(command, "NOTICE", 6)) {

				if (where == NULL || message == NULL) continue;

				if ((sep = strchr(user, '!')) != NULL) user[sep - user] = '\0';

				if (where[0] == '#' || where[0] == '&' || where[0] == '+' || where[0] == '!') target = where; else target = user;

				printf("[from: %s] [reply-with: %s] [where: %s] [reply-to: %s] %s", user, command, where, target, message);

				size_t page=0;
				char text[STRING_MAX];

				if(sscanf(msg,".kjv page %zu %[^\n]\n",&page,text)==2) {
				
					lex(&tokens,&ntokens,text);

					parse(infos,ninfos,tokens,ntokens,&cites,&ncites);

					Cites_Print(sock,channel,page,infos,ninfos,cites,ncites);

					Tokens_Free(&tokens,&ntokens);
					Cites_Free(&cites,&ncites);

				} else if(sscanf(msg,".kjv %[^\n]\n",text)==1) {
				
					lex(&tokens,&ntokens,text);

					parse(infos,ninfos,tokens,ntokens,&cites,&ncites);

					Cites_Print(sock,channel,1,infos,ninfos,cites,ncites);

					Tokens_Free(&tokens,&ntokens);
					Cites_Free(&cites,&ncites);
														
				} else if(sscanf(msg,".skjv page %zu %[^\n]\n",&page,text)==2) {
					search(sock,channel,page,text);	  					} else if(sscanf(msg,".skjv %[^\n]\n",text)==1) {
					search(sock,channel,1,text);								}
					
				//raw("%s %s :%s", command, target, message); // If you enable this the IRCd will get its "*** Looking up your hostname..." messages thrown back at it but it works...

			}
		}

		buf[0]='\0';

	}

	Infos_Free(&infos,&ninfos);
	
	return 0;
	
}


