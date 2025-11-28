#include <stdio.h>
#include "stride.tab.h"
extern int yylex(void);
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
YYSTYPE yylval;
int yyerror(const char *s){ fprintf(stderr, "yyerror: %s\n", s); return 0; }
int main(int argc,char **argv){
  if (argc<2) { fprintf(stderr,"usage: %s file\n", argv[0]); return 1; }
  FILE *f = fopen(argv[1],"r"); if(!f) { perror("fopen"); return 1; }
  yyin = f;
  int t;
  while((t = yylex()) != 0) {
    printf("TOKEN %d -> \"%s\" (line %d)\n", t, yytext?yytext:"", yylineno);
  }
  fclose(f);
  return 0;
}
