%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern int yylex(void);
extern FILE *yyin;
extern int yylineno;
int yydebug = 0;
void yyerror(const char *s);

/* codegen globals */
FILE *out = NULL;
int label_count = 0;
int new_label() { return ++label_count; }

void emit(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    vfprintf(out, fmt, ap);
    fprintf(out, "\n");
    va_end(ap);
}

/* symbol table */
typedef struct Sym {
  char *name;
  int slot;
  struct Sym *next;
} Sym;
Sym *symtable = NULL;
int next_slot = 0;
Sym *sym_find(const char *n) {
  for (Sym *s = symtable; s; s = s->next) if (strcmp(s->name, n)==0) return s;
  return NULL;
}
int sym_get_or_create(const char *n) {
  Sym *s = sym_find(n);
  if (s) return s->slot;
  s = malloc(sizeof(Sym));
  s->name = strdup(n);
  s->slot = next_slot++;
  s->next = symtable;
  symtable = s;
  return s->slot;
}

/* label stack for nested constructs */
#define LABEL_STACK_MAX 1024
int label_stack_else[LABEL_STACK_MAX];
int label_stack_end[LABEL_STACK_MAX];
int label_stack_top = 0;
void push_labels(int l_else, int l_end) {
  if (label_stack_top < LABEL_STACK_MAX) {
    label_stack_else[label_stack_top] = l_else;
    label_stack_end[label_stack_top] = l_end;
    label_stack_top++;
  }
}
void pop_labels() {
  if (label_stack_top > 0) label_stack_top--;
}
int top_label_else() { return label_stack_else[label_stack_top-1]; }
int top_label_end() { return label_stack_end[label_stack_top-1]; }

int is_sensor(const char *id) {
    if (!id) return 0;
    return (strcmp(id,"tempo")==0 || strcmp(id,"distancia")==0 || strcmp(id,"batimentos")==0);
}

%}

/* tokens */
%token ASSIGN
%token IF THEN ELSE END
%token WHILE DO
%token CORRA PARE ESPERE MOSTRE

%token IDENTIFIER
%token NUMBER
%token STRING

%token EQ NEQ LE GE

%debug
%left '+' '-'
%left '*' '/'

%union {
    int num;
    char *str;
}

%type <str> IDENTIFIER STRING
%type <num> NUMBER

%%

/* ---------- program / statements ---------- */
program:
    /* empty */
  | program statement
  ;

/* a statement is one top-level command */
statement:
    assignment
  | if_stmt
  | while_stmt
  | action
  ;

/* ---------- assignment ---------- */
assignment:
    IDENTIFIER ASSIGN expression
    {
        int slot = sym_get_or_create($1);
        emit("; ASSIGN %s := (top of stack)", $1);
        emit("POP R0");
        emit("STORE R0, @%d", slot);
        free($1);
    }
  ;

/* ---------- if ---------- */
/* now we require a simple condition: term COMP term */
if_stmt:
    IF condition THEN stmt_list END
    {
      /* no else */
      /* condition already emitted code that jumps to L? if false */
      /* after then-block, nothing else */
    }
  | IF condition THEN stmt_list ELSE stmt_list END
    {
      /* with else: condition already emitted code that jumps to L_else if false.
         we need to jump over else and emit labels; condition code used top_label_end() and top_label_else() */
      /* the condition emission created labels earlier */
    }
  ;

/* ---------- while ---------- */
while_stmt:
    WHILE
    {
      /* create labels and emit start */
      int l_start = new_label();
      int l_end = new_label();
      push_labels(l_start, l_end);
      emit("L%d:", l_start);
    }
    condition
    DO stmt_list END
    {
      /* at end of body, jump back to start and emit end label */
      int l_start = top_label_else();
      int l_end   = top_label_end();
      emit("JMP L%d", l_start);
      emit("L%d:", l_end);
      pop_labels();
    }
  ;

/* ---------- stmt_list (zero or more statements) ---------- */
stmt_list:
    /* empty */ { }
  | stmt_list statement { }
  ;

/* ---------- actions ---------- */
action:
    CORRA  { emit("; ACTION corra"); emit("PRINT \"ACTION corra\""); }
  | PARE   { emit("; ACTION pare"); emit("PRINT \"ACTION pare\""); }
  | ESPERE expression
      {
        emit("; ACTION espere (pop seconds)");
        emit("POP R0");
        emit("WAIT R0");
      }
  | MOSTRE expression
      {
        emit("; ACTION mostre (pop and print)");
        emit("POP R0");
        emit("PRINT R0");
      }
  ;

/* ---------- condition: simple comparator between two terms ---------- */
condition:
    /* we evaluate left term then right term, then compare and jump to 'false' label */
    term '<' term
    {
      /* labels must have been pushed by the surrounding if/while */
      int l_false = top_label_else();
      /* terms push values onto stack */
      emit("; COND <");
      emit("POP R1");
      emit("POP R0");
      emit("CMP R0, R1");
      /* if NOT (R0 < R1) then jump to false (i.e., R0 >= R1 -> JGE) */
      emit("JGE L%d", l_false);
    }
  | term '>' term
    {
      int l_false = top_label_else();
      emit("; COND >");
      emit("POP R1");
      emit("POP R0");
      emit("CMP R0, R1");
      /* if NOT (R0 > R1) then R0 <= R1 -> JLE */
      emit("JLE L%d", l_false);
    }
  | term EQ term
    {
      int l_false = top_label_else();
      emit("; COND ==");
      emit("POP R1");
      emit("POP R0");
      emit("CMP R0, R1");
      /* if NOT equal -> JNZ to false? CMP JNZ means not zero -> jump if not equal */
      emit("JNZ L%d", l_false);
    }
  | term NEQ term
    {
      int l_false = top_label_else();
      emit("; COND !=");
      emit("POP R1");
      emit("POP R0");
      emit("CMP R0, R1");
      /* if NOT (R0 != R1) then equal -> JZ false */
      emit("JZ L%d", l_false);
    }
  | term LE term
    {
      int l_false = top_label_else();
      emit("; COND <=");
      emit("POP R1");
      emit("POP R0");
      emit("CMP R0, R1");
      /* if NOT (R0 <= R1) then R0 > R1 -> JGT */
      emit("JGT L%d", l_false);
    }
  | term GE term
    {
      int l_false = top_label_else();
      emit("; COND >=");
      emit("POP R1");
      emit("POP R0");
      emit("CMP R0, R1");
      /* if NOT (R0 >= R1) then R0 < R1 -> JLT */
      emit("JLT L%d", l_false);
    }
  ;

/* ---------- expressions (binary ops, precedence) ---------- */
expression:
    expr_add
    ;

expr_add:
    expr_add '+' expr_mul
    {
      emit("; BINOP +");
      emit("POP R1");
      emit("POP R0");
      emit("ADD R0, R1");
      emit("PUSH R0");
    }
  | expr_add '-' expr_mul
    {
      emit("; BINOP -");
      emit("POP R1");
      emit("POP R0");
      emit("SUB R0, R1");
      emit("PUSH R0");
    }
  | expr_mul
    { /* pass-through */ }
  ;

expr_mul:
    expr_mul '*' term
    {
      emit("; BINOP *");
      emit("POP R1");
      emit("POP R0");
      emit("MUL R0, R1");
      emit("PUSH R0");
    }
  | expr_mul '/' term
    {
      emit("; BINOP /");
      emit("POP R1");
      emit("POP R0");
      emit("DIV R0, R1");
      emit("PUSH R0");
    }
  | term
    { /* pass-through */ }
  ;

/* ---------- terms ---------- */
term:
    NUMBER
    {
      emit("; TERM number %d", $1);
      emit("MOV R0, %d", $1);
      emit("PUSH R0");
    }
  | IDENTIFIER
    {
      if (is_sensor($1)) {
        if (strcmp($1,"tempo")==0) emit("READ_SENSOR R0, TEMPO");
        else if (strcmp($1,"distancia")==0) emit("READ_SENSOR R0, DISTANCIA");
        else if (strcmp($1,"batimentos")==0) emit("READ_SENSOR R0, BATAMENTOS");
        emit("PUSH R0");
        free($1);
      } else {
        int slot = sym_get_or_create($1);
        emit("; TERM identifier %s -> slot @%d", $1, slot);
        emit("LOAD R0, @%d", slot);
        emit("PUSH R0");
        free($1);
      }
    }
  | STRING
    {
      emit("; TERM string \"%s\"", $1);
      emit("MOV R0, \"%s\"", $1);
      emit("PUSH R0");
      free($1);
    }
  ;

%%

/* error reporting */
void yyerror(const char *s) {
    fprintf(stderr, "Erro (linha %d): %s\n", yylineno, s);
}

/* main: parse to temp and commit only on success */
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s fonte.stride [saida.asm]\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) { perror("fopen fonte"); return 1; }

    const char *outname = (argc > 2) ? argv[2] : "out.asm";
    const char *tmpname = "out_temp.asm";
    out = fopen(tmpname, "w");
    if (!out) { perror("fopen out"); fclose(yyin); return 1; }

    emit("; Stride compiled assembly");
    emit("; sensors available: TEMPO DISTANCIA BATAMENTOS");
    emit("; memory slots start at @0");
    emit("; --- begin program ---");

    /* ativar trace se necessário:
       yydebug = 1;
    */

    int parse_result = yyparse();

    if (parse_result == 0) {
        emit("HALT");
        fclose(out);
        fclose(yyin);
        if (rename(tmpname, outname) != 0) { perror("rename out temp"); return 1; }
        printf("[COMPILADO] %s -> %s\n", argv[1], outname);
        return 0;
    } else {
        fprintf(stderr, "[ERRO] parse falhou. Arquivo de saída não será escrito.\n");
        fclose(out);
        fclose(yyin);
        remove(tmpname);
        return 1;
    }
}
