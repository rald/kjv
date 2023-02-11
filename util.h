#ifndef UTIL_H
#define UTIL_H

#define _GNU_SOURCE

#include <string.h>
#include <sys/types.h>

#define IRC_IMPLEMENTATION
#include "irc.h"

#define INFO_IMPLEMENTATION
#include "info.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"


size_t getbnum(Info **infos,size_t ninfos,char *bname);

char *getbname(Info **infos,size_t ninfos,size_t bnum);

void search(int conn,char *chn,size_t page,char *text);


#ifdef UTIL_IMPLEMENTATION



size_t getbnum(Info **infos,size_t ninfos,char *bname) {
  if(bname && *bname) {
    for(size_t j=0;j<ninfos;j++) {
      if(strcasecmp(bname,infos[j]->bname)==0) {
        return infos[j]->bnum;
      }
      for(size_t k=0;k<infos[j]->nbsnames;k++) {
        if(strcasecmp(bname,infos[j]->bsnames[k])==0) {
          return infos[j]->bnum;
        }      
      }
    }
  }
  return 0;
}



char *getbname(Info **infos,size_t ninfos,size_t bnum) {
  for(size_t j=0;j<ninfos;j++) {
    if(bnum==infos[j]->bnum) {
      return infos[j]->bname;
    }
  }
  return NULL;
}



void search(int conn,char *chn,size_t page,char *text) {

	trim(text);

 	if(text && *text) {

		FILE *fp=fopen("kjv.csv","r");

		char *line=NULL;
		size_t llen=0;
		ssize_t rlen=0;

		size_t p=1;
		size_t c=0;
		size_t n=0;

		while((rlen=getline(&line,&llen,fp)) && rlen>0) {
			char **tokens=NULL;	
		 	size_t ntokens=0;

			rmnl(line);
			trim(line);

			tokenize(&tokens,&ntokens,line,"|");

			char *bname=tokens[0];
			size_t cnum=atoi(tokens[1]);
			size_t vnum=atoi(tokens[2]);
			char *vers=tokens[3];

			char passage[1024];

			sprintf(passage,"%s %zu:%zu %s",bname,cnum,vnum,vers);

			if(strcasestr(passage,text)) {

				n++;
					
				if(p==page) privmsg(conn,chn,"(page %zu # %zu) %s %zu:%zu -> %s\n",p,n,bname,cnum,vnum,vers);
				c++; if(c>=4) { c=0; p++; }
			}

			tokfree(&tokens,&ntokens);

			free(line);
			line=NULL;
			llen=0;
			rlen=0;
		}

		raw(conn,"PRIVMSG %s :found %zu in %zu %s\r\n",chn,n,p,p<2?"page":"pages");

		free(line);
		line=NULL;
		llen=0;
		rlen=0;

		fclose(fp);

	}
}

#endif

#endif
