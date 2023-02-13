#ifndef STRUTIL_H
#define STRUTIL_H

#include <string.h>
#include <ctype.h>



char *trim(char *a);

void tokenize(char ***tokens,size_t *ntokens,char *s,char *d);

void tokfree(char ***tokens,size_t *ntokens);

char *rmnl(char *s);



#ifdef STRUTIL_IMPLEMENTATION



char *trim(char *a) {
    char *p=a,*q=a;
    while(isspace(*q)) ++q;
    while(*q) *p++=*q++;
    *p='\0';
    while(p>a && isspace(*--p)) *p='\0';
    return a;
}



void tokenize(char ***tokens,size_t *ntokens,char *s,char *d) {
  char *token=strtok(s,d);
  while(token) {
    *tokens=realloc(*tokens,sizeof(**tokens)*(*ntokens+1));
    (*tokens)[(*ntokens)++]=token?strdup(token):NULL;
    token=strtok(NULL,d);
  }
}



void tokfree(char ***tokens,size_t *ntokens) {
  for(size_t i=0;i<*ntokens;i++) {
    if((*tokens)[i]) {
    	free((*tokens)[i]);
    }
    (*tokens)[i]=NULL;
  }
  free(*tokens);
  *ntokens=0;
}



char *rmnl(char *s) {
  char *p=strchr(s,'\n');
  if(p) *p='\0';
  return s;
}



static char *skip(char *s, char c) {
	while (*s != c && *s != '\0')
		s++;
	if (*s != '\0')
		*s++ = '\0';
	else
		return NULL;
	return s;
}



#endif /* STRUTIL_IMPLEMENTATION */



#endif /* STRUTIL_H */


