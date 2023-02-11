#ifndef INFO_H
#define INFO_H



#define STRUTIL_IMPLEMENTATION
#include "strutil.h"



typedef struct Info Info;

struct Info {
  char *bname;
  char **bsnames;
  size_t nbsnames;
  size_t bnum;
  size_t nchap;
  size_t *nvers;
};



Info *Info_New();

void Infos_Free(Info ***infos,size_t *ninfos);

void Info_Append(Info ***infos,size_t *ninfos,Info *info);

void Info_Load(Info ***infos,size_t *ninfos,char *filename);

void Info_Print(Info **infos,size_t ninfos);



#ifdef INFO_IMPLEMENTATION



Info *Info_New() {
  Info *info=malloc(sizeof(*info));
  if(info) {
    info->bname=NULL;
    info->bsnames=NULL;
    info->nbsnames=0;
    info->bnum=0;
    info->nchap=0;
    info->nvers=NULL;    
  }
  return info;
}


void Infos_Free(Info ***infos,size_t *ninfos) {
	for(size_t i=0;i<*ninfos;i++) {
		free((*infos)[i]->bname);
		(*infos)[i]->bname=NULL;

		tokfree(&(*infos)[i]->bsnames,&(*infos)[i]->nbsnames);

		(*infos)[i]->bnum=0;

		free((*infos)[i]->nvers);
		(*infos)[i]->nvers=NULL;

		(*infos)[i]->nchap=0;
		
	 	free((*infos)[i]);;
		(*infos)[i]=NULL;
	}
	free(*infos);
	*infos=NULL;
	*ninfos=0;
}



void Info_Append(Info ***infos,size_t *ninfos,Info *info) {
  (*infos)=realloc(*infos,sizeof(**infos)*((*ninfos)+1));
  (*infos)[(*ninfos)++]=info;
}



void Info_Load(Info ***infos,size_t *ninfos,char *filename) {
  FILE *fp=fopen(filename,"r");

  char *line=NULL;
  size_t llen=0;
  ssize_t rlen=0;
 
  while((rlen=getline(&line,&llen,fp)) && rlen>0) {

	  char **tokens0=NULL;
	  size_t ntokens0=0;
	  char **tokens1=NULL;
	  size_t ntokens1=0;
   
    tokenize(&tokens0,&ntokens0,line,"|");

    Info *info=Info_New();

    info->bname=strdup(tokens0[0]);

    tokenize(&info->bsnames,&info->nbsnames,tokens0[1],"/");
    info->bnum=atoi(tokens0[2]);
    info->nchap=atoi(tokens0[3]);
    
    tokenize(&tokens1,&ntokens1,tokens0[4],",");

    info->nvers=malloc(sizeof(*info->nvers)*info->nchap);

    for(size_t i=0;i<info->nchap;i++) {
      info->nvers[i]=atoi(tokens1[i]);
    }
    
    Info_Append(infos,ninfos,info);
    
    tokfree(&tokens1,&ntokens1);
    tokfree(&tokens0,&ntokens0);

		free(line);
	 	line=NULL;
	  llen=0;
	  rlen=0;
  }

	free(line);
 	line=NULL;
  llen=0;
  rlen=0;

	fclose(fp);
  
}



void Info_Print(Info **infos,size_t ninfos) {
  for(size_t i=0;i<ninfos;i++) {
    printf("%s|",infos[i]->bname);
    for(size_t j=0;j<infos[i]->nbsnames;j++) {
      if(j!=0) printf("/");
      printf("%s",infos[i]->bsnames[j]);
    }
    printf("|%zu|%zu|",infos[i]->bnum,infos[i]->nchap);
    for(size_t j=0;j<infos[i]->nchap;j++) {
      if(j!=0) printf(",");
      printf("%zu",infos[i]->nvers[j]);
    }
    printf("\n");
  }
}


#endif /* INFO_IMPLEMENTATION */



#endif /* IMFO_H */

