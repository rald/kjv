bibly: main.c common.h irc.h strutil.h util.h info.h token.h lexer.h cite.h parser.h 
	gcc main.c -o bibly -g

clean:
	rm bibly
