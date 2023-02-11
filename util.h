#ifndef UTIL_H
#define UTIL_H

#define _GNU_SOURCE

#include <string.h>
#include <sys/types.h>

#define INFO_IMPLEMENTATION
#include "info.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"


size_t getbnum(Info **infos,size_t ninfos,char *bname);

char *getbname(Info **infos,size_t ninfos,size_t bnum);

void search(char *text);



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



void search(char *text) {

	trim(text);

 	if(text && *text) {

		FILE *fp=fopen("kjv.csv","r");

		char *line=NULL;
		size_t llen=0;
		ssize_t rlen=0;

		bool first=true;

		printf("\n");

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
				if(first) first=false; else printf("\n");
				printf("%s %zu:%zu -> %s\n",bname,cnum,vnum,vers);
			}

			tokfree(&tokens,&ntokens);

			free(line);
			line=NULL;
			llen=0;
			rlen=0;
		}

		printf("\n");

		free(line);
		line=NULL;
		llen=0;
		rlen=0;

		fclose(fp);

	}
}

#endif

#endif
