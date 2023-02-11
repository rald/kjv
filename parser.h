#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"

#define CITE_IMPLEMENTATION
#include "cite.h"



void parse(Info **infos,size_t ninfos,Token **token,size_t ntokens,Cite ***cites,size_t *ncites);



#ifdef PARSER_IMPLEMENTATION



static size_t i=0;
static Cite *cite=NULL;

void p0(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites);

void p1(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites);

void p2(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites);

void p3(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites);


void p3(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites) {

  while(i<ntokens && tokens[i]->type==TOKENTYPE_COMMA) {
    i+=1;
    if(i<ntokens && tokens[i]->type==TOKENTYPE_STRING) {    
      cite->bnum=0;
      cite->scnum=0;
      cite->ecnum=0;
      cite->svnum=0;
      cite->evnum=0;
      
      p0(infos,ninfos,tokens,ntokens,cites,ncites);
    } else if(i+1<ntokens && tokens[i]->type==TOKENTYPE_INTEGER && tokens[i+1]->type==TOKENTYPE_STRING) {          
      cite->bnum=0;
      cite->scnum=0;
      cite->ecnum=0;
      cite->svnum=0;
      cite->evnum=0;

      p0(infos,ninfos,tokens,ntokens,cites,ncites);
    } else if(i+1<ntokens && tokens[i]->type==TOKENTYPE_INTEGER  && tokens[i+1]->type==TOKENTYPE_COLON) {          
      cite->scnum=0;
      cite->ecnum=0;
      cite->svnum=0;
      cite->evnum=0;

      p1(infos,ninfos,tokens,ntokens,cites,ncites);

      Cite_Append(cites,ncites,Cite_New(cite->bnum,cite->scnum,cite->ecnum,cite->svnum,cite->evnum));
      
    } else if(i<ntokens && tokens[i]->type==TOKENTYPE_INTEGER) {          

      cite->ecnum=0;
      cite->svnum=0;
      cite->evnum=0;

      p2(infos,ninfos,tokens,ntokens,cites,ncites);      
      Cite_Append(cites,ncites,Cite_New(cite->bnum,cite->scnum,cite->ecnum,cite->svnum,cite->evnum));
    } 

  }
}


void p2(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites) {
  if(tokens[i]->type!=TOKENTYPE_EOF) {
    if( 
        i+2<ntokens 
        && tokens[i]->type==TOKENTYPE_INTEGER
        && tokens[i+1]->type==TOKENTYPE_DASH
        && tokens[i+2]->type==TOKENTYPE_INTEGER
    ) {
      cite->svnum=atoi(tokens[i]->text);
      cite->evnum=atoi(tokens[i+2]->text);
      i+=3;    
    } else if (  
        i<ntokens 
        && tokens[i]->type==TOKENTYPE_INTEGER
    ) {
      cite->svnum=atoi(tokens[i]->text);      
      cite->evnum=0;
      i+=1;
    } 
  }
}



void p1(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites) {
  if(tokens[i]->type!=TOKENTYPE_EOF) {
    if( 
        i+2<ntokens 
        && tokens[i]->type==TOKENTYPE_INTEGER
        && tokens[i+1]->type==TOKENTYPE_DASH
        && tokens[i+2]->type==TOKENTYPE_INTEGER
    ) {
      cite->scnum=atoi(tokens[i]->text);
      cite->ecnum=atoi(tokens[i+2]->text);  
      i+=3;    
    } else if (  
        i<ntokens 
        && tokens[i]->type==TOKENTYPE_INTEGER
    ) {
      cite->scnum=atoi(tokens[i]->text);      
      cite->ecnum=0;
      i+=1;

      if(i<ntokens && tokens[i]->type==TOKENTYPE_COLON) {
        i+=1;
        p2(infos,ninfos,tokens,ntokens,cites,ncites);
      }
    } 
  }  
}



void p0(Info **infos,size_t ninfos,Token **tokens,size_t ntokens,Cite ***cites,size_t *ncites) {

  if(tokens[i]->type!=TOKENTYPE_EOF) {  

		if(i<ntokens && tokens[i]->type==TOKENTYPE_COMMA) {
			p3(infos,ninfos,tokens,ntokens,cites,ncites);
		} else {

	    char bname[STRING_MAX];
	    bname[0]='\0';

	    if(i<ntokens && tokens[i]->type==TOKENTYPE_STRING) {
	      strcpy(bname,tokens[i]->text);
	      i+=1;
	    } else if(i+1<ntokens && tokens[i]->type==TOKENTYPE_INTEGER && tokens[i+1]->type==TOKENTYPE_STRING) {
	      sprintf(bname,"%s %s",tokens[i]->text,tokens[i+1]->text);  
	      i+=2;
	    } 
	    
	    if((cite->bnum=getbnum(infos,ninfos,bname))) {

	      p1(infos,ninfos,tokens,ntokens,cites,ncites);

	     	Cite_Append(cites,ncites,Cite_New(cite->bnum,cite->scnum,cite->ecnum,cite->svnum,cite->evnum));

	      p3(infos,ninfos,tokens,ntokens,cites,ncites);

			}
		}
  }
}




void parse(Info **infos,size_t ninfos,Token **token,size_t ntokens,Cite ***cites,size_t *ncites) {
	i=0;

	cite=Cite_New(0,0,0,0,0);

  p0(infos,ninfos,token,ntokens,cites,ncites);
 	Cite_Free(&cite);
}



#endif /* PARSER_IMPLEMENTATION */



#endif /* PARSER_H */



