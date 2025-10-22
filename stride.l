%option noyywrap
%{
#include "stride.tab.h"
#include <string.h>
#include <stdlib.h>
int line_no = 1;
static char *make_string(const char *s){return strdup(s);}
%}
LETTER  [A-Za-z_]
DIGIT   [0-9]
ID      {LETTER}({LETTER}|{DIGIT}|_)*

%%
[ \t\r]+                ;
\n                      { line_no++; }
"//".*                  ;
\"[^\n"]*\"             {int len=yyleng;char *s=(char*)malloc(len-1);if(len>=2){strncpy(s,yytext+1,len-2);s[len-2]='\0';}else{s[0]='\0';}yylval.str=s;return STRING_LITERAL;}
{DIGIT}+                {yylval.number=atoi(yytext);return NUMBER;}
"corra"                 {return CORRA;}
"pare"                  {return PARE;}
"espere"                {return ESPERE;}
"mostre"                {return MOSTRE;}
"if"                    {return IF;}
"then"                  {return THEN;}
"else"                  {return ELSE;}
"end"                   {return END;}
"while"                 {return WHILE;}
"do"                    {return DO;}
"tempo"                 {return TEMPO;}
"distancia"             {return DISTANCIA;}
"batimentos"            {return BATIMENTOS;}
{ID}                    {yylval.str=make_string(yytext);return IDENTIFIER;}
":="                    {return ASSIGN;}
"=="                    {return EQ;}
"!="                    {return NEQ;}
"<="                    {return LE;}
">="                    {return GE;}
"<"                     {return '<';}
">"                     {return '>';}
"+"                     {return '+';}
"-"                     {return '-';}
"*"                     {return '*';}
"/"                     {return '/';}
"("                     {return '(';}
")"                     {return ')';}
";"                     {return ';';}
","                     {return ',';}
.                       {return yytext[0];}
%%

