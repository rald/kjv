#ifndef TOKEN_H
#define TOKEN_H 

#include "common.h"

typedef enum TokenType {
  TOKENTYPE_UNKNOWN=0,
  TOKENTYPE_STRING,
  TOKENTYPE_INTEGER,
  TOKENTYPE_SPACE,
  TOKENTYPE_COLON,
  TOKENTYPE_DASH,  
  TOKENTYPE_COMMA,
  TOKENTYPE_EOF,
  TOKENTYPE_MAX
} TokenType;

char *tokenNames[]={
  "TOKENTYPE_UNKNOWN",
  "TOKENTYPE_STRING",
  "TOKENTYPE_INTEGER",
  "TOKENTYPE_SPACE",
  "TOKENTYPE_COLON",
  "TOKENTYPE_DASH", 
  "TOKENTYPE_COMMA",
  "TOKENTYPE_EOF"
};

typedef struct Token Token;

struct Token {
  TokenType type;
  char *text;
};


Token *Token_New(TokenType type,char *text);

void Tokens_Free(Token ***tokens,size_t *ntokens);

void Token_Append(Token ***tokens,size_t *ntokens,TokenType type,char *text);

char *Token_ToString(Token *token);



#ifdef TOKEN_IMPLEMENTATION



Token *Token_New(TokenType type,char *text) {
  Token *token=malloc(sizeof(Token));
  if(token) {
    token->type=type;
    token->text=text?strdup(text):NULL;
  }
  return token;
}



void Tokens_Free(Token ***tokens,size_t *ntokens) {
  for(size_t i=0;i<*ntokens;i++) {
    if((*tokens)[i]->text) {
      free((*tokens)[i]->text);
    }
    (*tokens)[i]->text=NULL;
    free((*tokens)[i]);
    (*tokens)[i]=NULL;    
  }
  free(*tokens);
  *tokens=NULL;
  *ntokens=0;
}



void Token_Append(Token ***tokens,size_t *ntokens,TokenType type,char *text) {
  (*tokens)=realloc(*tokens,sizeof(**tokens)*((*ntokens)+1));
  (*tokens)[(*ntokens)++]=Token_New(type,text);
}


char *Token_ToString(Token *token) {
  char *str=malloc(sizeof(char)*STRING_MAX);
  sprintf(str,"{ type: %s, text: \"%s\" }",tokenNames[token->type],token->text);
  return str;
}


#endif /* TOKEN_IMPLEMENTATION */



#endif /* TOKEN_H */
