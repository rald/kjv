#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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



#include "common.h"

#define INI_IMPLEMENTATION 
#include "ini.h"

#define IRC_IMPLEMENTATION 
#include "irc.h"

#define STRUTIL_IMPLEMENTATION 
#include "strutil.h"

#define INFO_IMPLEMENTATION 
#include "info.h"

#define TOKEN_IMPLEMENTATION 
#include "token.h"

#define LEXER_IMPLEMENTATION 
#include "lexer.h"

#define PARSER_IMPLEMENTATION 
#include "parser.h"

#define CITE_IMPLEMENTATION 
#include "cite.h"


const char *mst = "siesta";
const char *hst = "irc.undernet.org";
const char *prt = "6667";
const char *nck = "siestu";
const char *chn = "#pantasya";
const char *pss = NULL;



int sck;

typedef struct IrcMsg IrcMsg;

struct IrcMsg {
	char *usr;
	char *cmd;
	char *par; 
	char *txt;
};



static Info **infos=NULL;
static size_t ninfos=0;

static Token **tokens=NULL;
static size_t ntokens=0;

static Cite **cites=NULL;
static size_t ncites=0;




void parsein(IrcMsg *im) {

	char *msg=strdup(im->txt);

	trim(msg);

	size_t page=0;
	char text[STRING_MAX];

	if(sscanf(msg,".kjv page %zu %[^\n]\n",&page,text)==2) {
		lex(&tokens,&ntokens,text);
		parse(infos,ninfos,tokens,ntokens,&cites,&ncites);
		Cites_Print(sck,chn,page,infos,ninfos,cites,ncites);
		Tokens_Free(&tokens,&ntokens);
		Cites_Free(&cites,&ncites);
	} else if(sscanf(msg,".kjv %[^\n]\n",text)==1) {
		lex(&tokens,&ntokens,text);
		parse(infos,ninfos,tokens,ntokens,&cites,&ncites);
		Cites_Print(sck,chn,1,infos,ninfos,cites,ncites);
		Tokens_Free(&tokens,&ntokens);
		Cites_Free(&cites,&ncites);
	} else if(sscanf(msg,".skjv page %zu %[^\n]\n",&page,text)==2) {
		search(sck,chn,page,text);	  					
	} else if(sscanf(msg,".skjv %[^\n]\n",text)==1) {
		search(sck,chn,1,text);
	}
					
	free(msg);
	msg=NULL;

}



void parsesrv(IrcMsg *im,char *line) {
	im->cmd=line;
	im->usr = (char*)hst;
	if(!im->cmd || !*im->cmd) return; 
	if(im->cmd[0] == ':') {
		im->usr = im->cmd + 1;
		im->cmd = skip(im->usr, ' ');
		if(!*im->cmd) return;
		skip(im->usr, '!');
	}
	skip(im->cmd, '\r');
	im->par = skip(im->cmd, ' ');
	im->txt = skip(im->par, ':');
	trim(im->par);
}



int main(void) {

	char line[STRING_MAX];
	size_t llen=STRING_MAX;
	ssize_t rlen=0;

	IrcMsg im;

	srand(time(NULL));


	Info_Load(&infos,&ninfos,"kjv.inf");


  ini_t *config = ini_load("config.ini");

  const char *val; 

  val = ini_get(config,"default","host");

  hst=val?val:hst;

  val = ini_get(config,"default","port");

  prt=val?val:prt;

  val = ini_get(config,"default","nick");

  nck=val?val:nck;

  val = ini_get(config,"default","password");

  pss=val?val:pss;

  val = ini_get(config,"default","channel");

  chn=val?val:chn;

  val = ini_get(config,"default","master");

  mst=val?val:mst;



	sck=Irc_Connect(hst,prt);

  if (sck<0) {
  	DieWithUserMessage("Irc_Connect() failed", "unable to connect");
  }

	if(pss) raw(sck,"PASS %s\r\n",pss);
	raw(sck,"NICK %s\r\n",nck);
	raw(sck,"USER %s %s %s :%s\r\n",nck,nck,nck,nck);

	while((rlen=Irc_Recv(sck,line,llen))!=-1) { 

		if(rlen>0) {

			im.cmd=NULL;
			im.usr=NULL;
			im.par=NULL;
			im.txt=NULL;

			parsesrv(&im,line);		

	/*
			printf("cmd: %s\n",im.cmd);
			printf("usr: %s\n",im.usr);
			printf("par: %s\n",im.par);
			printf("txt: %s\n",im.txt);
	//*/
			
			if(!strcmp(im.cmd,"PING")) {
				line[1]='O';
				raw(sck,"PONG :%s\r\n",im.txt);
			} else if(!strcmp(im.cmd,"001")) {
				raw(sck,"JOIN %s\r\n",chn);
			} else if(!strcmp(im.cmd,"PRIVMSG")) {
				printf("%s <%s> %s\n",im.par,im.usr,im.txt);
				parsein(&im);				
			}

		}

		line[0]='\0';
		llen=STRING_MAX;
		rlen=0;

  }

	Infos_Free(&infos,&ninfos);

	return 0;
	
}


