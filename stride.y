%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int yylex(void);
extern FILE *yyin;
void yyerror(const char *s);
extern int line_no;
%}
%union {int number;char *str;}
%token <number> NUMBER
%token <str> IDENTIFIER
%token <str> STRING_LITERAL
%token CORRA PARE ESPERE MOSTRE
%token IF THEN ELSE END WHILE DO
%token TEMPO DISTANCIA BATIMENTOS
%token EQ NEQ LE GE
%token ASSIGN
%left '+' '-'
%left '*' '/'
%nonassoc '<' '>' EQ NEQ LE GE
%right UMINUS
%type <number> expression term factor

%%
program:
      decl_or_command_list
    ;
decl_or_command_list:
      decl_or_command
    | decl_or_command_list decl_or_command
    ;
decl_or_command:
      assignment
    | if_stmt
    | while_stmt
    | action_stmt
    ;
assignment:
    IDENTIFIER ASSIGN expression ';'   {printf("assignment: %s := %d\n",$1,$3);free($1);}
    ;
if_stmt:
    IF expression THEN decl_or_command_list opt_else END
    ;
opt_else:
    | ELSE decl_or_command_list
    ;
while_stmt:
    WHILE expression opt_do decl_or_command_list END
    ;
opt_do:
    | DO
    ;
action_stmt:
      CORRA ';'                         {printf("action: corra\n");}
    | PARE ';'                          {printf("action: pare\n");}
    | ESPERE expression ';'             {printf("action: espere %d\n",$2);}
    | MOSTRE expression ';'             {printf("action: mostre expr => %d\n",$2);}
    | MOSTRE STRING_LITERAL ';'         {printf("action: mostre string => %s\n",$2);free($2);}
    ;
expression:
      expression '+' term              {$$=$1+$3;}
    | expression '-' term              {$$=$1-$3;}
    | term                             {$$=$1;}
    | expression '<' expression        {$$=($1<$3);}
    | expression '>' expression        {$$=($1>$3);}
    | expression EQ expression         {$$=($1==$3);}
    | expression NEQ expression        {$$=($1!=$3);}
    | expression LE expression         {$$=($1<=$3);}
    | expression GE expression         {$$=($1>=$3);}
    ;
term:
      term '*' factor                  {$$=$1*$3;}
    | term '/' factor                  {$$=$1/$3;}
    | factor                           {$$=$1;}
    ;
factor:
      NUMBER                           {$$=$1;}
    | IDENTIFIER                       {$$=0;free($1);}
    | TEMPO                            {$$=0;}
    | DISTANCIA                        {$$=0;}
    | BATIMENTOS                       {$$=0;}
    | '(' expression ')'               {$$=$2;}
    | '-' factor %prec UMINUS          {$$=-$2;}
    ;
%%
void yyerror(const char *s){fprintf(stderr,"Erro: %s na linha %d\n",s,line_no);}
int main(int argc,char **argv){
    if(argc>1){
        yyin=fopen(argv[1],"r");
        if(!yyin){perror("fopen");return 1;}
    }else{
        yyin=stdin;
    }
    yyparse();
    return 0;
}
