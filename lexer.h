#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <ctype.h>

#include "common.h"

#define STRUTIL_IMPLEMENTATION
#include "strutil.h"

#define TOKEN_IMPLEMENTATION
#include "token.h"



typedef enum LexerState {
  LEXERSTATE_DEFAULT=0,
  LEXERSTATE_STRING,
  LEXERSTATE_INTEGER,
  LEXERSTATE_MAX,  
} LexerState;



void lex(Token ***tokens,size_t *ntokens,char *str);



#ifdef LEXER_IMPLEMENTATION



void lex(Token ***tokens,size_t *ntokens,char *str) {

  LexerState lexerState=LEXERSTATE_DEFAULT;

  size_t i=0;
  char text[STRING_MAX];
  int c;
  bool quit=false;

  text[0]='\0';

  while(!quit) {
    c=str[i];
    switch(lexerState) {
      case LEXERSTATE_DEFAULT:
        if(c=='\0') {
          quit=true;
        } else if(isalpha(c)) {
          lexerState=LEXERSTATE_STRING;
          i--;
        } else if(isdigit(c)) {
          lexerState=LEXERSTATE_INTEGER;
          i--;
        } else if(c==':') {
          Token_Append(tokens,ntokens,TOKENTYPE_COLON,(char[2]){c,'\0'});     
        } else if(c=='-') {
          Token_Append(tokens,ntokens,TOKENTYPE_DASH,(char[2]){c,'\0'});     
        } else if(c==',') {
          Token_Append(tokens,ntokens,TOKENTYPE_COMMA,(char[2]){c,'\0'});     
        }
      break;
      case LEXERSTATE_STRING:
        if(c!='\0' && (isalpha(c) || isspace(c))) {
          strcat(text,(char[2]){c,'\0'});
        } else {
          Token_Append(tokens,ntokens,TOKENTYPE_STRING,trim(text));
          text[0]='\0';
          i--;
          lexerState=LEXERSTATE_DEFAULT;
        }
      break;
      case LEXERSTATE_INTEGER:
        if(c!='\0' && isdigit(c)) {
          strcat(text,(char[2]){c,'\0'});
        } else {
          Token_Append(tokens,ntokens,TOKENTYPE_INTEGER,text);
          text[0]='\0';
          i--;
          lexerState=LEXERSTATE_DEFAULT;
        }
      break;
      default: break;
    }
    i++;
  }

  Token_Append(tokens,ntokens,TOKENTYPE_EOF,NULL);  
}



#endif /* LEXER_IMPLEMENTATION */



#endif /* LEXER_H */
