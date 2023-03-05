bibly: main.c common.h ini.h irc.h strutil.h util.h bibly.h info.h token.h lexer.h parser.h cite.h
	gcc main.c -o bibly -lm -g -Wall -Wextra -pedantic

clean:
	rm bibly
