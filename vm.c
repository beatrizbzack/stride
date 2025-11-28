// vm.c - simple interpreter for Stride assembly
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROG 10000
#define MAX_LINE 512
#define MEM_SIZE 1024
#define STACK_SIZE 1024

typedef struct {
    int is_str;
    long num;
    char *str;
} Value;

typedef struct {
    char *op;
    char *a;
    char *b;
} Instr;

Instr prog[MAX_PROG];
int prog_len = 0;
int pc = 0;

Value mem[MEM_SIZE];
Value stack_vals[STACK_SIZE];
int sp = 0; // next free index; push -> stack_vals[sp++]

Value R0, R1, R2;

int cmp_flag = 0; // negative: -1, zero:0, positive:1

// sensors (can be updated or read from env)
long sensor_tempo = 100;
long sensor_distancia = 5;
long sensor_batimentos = 70;

void push_val(Value v) {
    if (sp >= STACK_SIZE) { fprintf(stderr, "Stack overflow\n"); exit(1); }
    stack_vals[sp++] = v;
}
Value pop_val() {
    if (sp <= 0) { fprintf(stderr, "Stack underflow\n"); exit(1); }
    return stack_vals[--sp];
}

Value make_num(long n) { Value v; v.is_str = 0; v.num = n; v.str = NULL; return v; }
Value make_str(const char *s) { Value v; v.is_str = 1; v.str = strdup(s); v.num = 0; return v; }

char *trim(char *s) {
    while(isspace((unsigned char)*s)) s++;
    char *end = s + strlen(s) - 1;
    while(end > s && isspace((unsigned char)*end)) { *end = '\0'; end--; }
    return s;
}

int is_register(const char *s) {
    if (!s) return 0;
    return (strcmp(s,"R0")==0 || strcmp(s,"R1")==0 || strcmp(s,"R2")==0);
}

Value *reg_ptr(const char *r) {
    if (strcmp(r,"R0")==0) return &R0;
    if (strcmp(r,"R1")==0) return &R1;
    return &R2;
}

void set_cmp(Value *a, Value *b) {
    if (a->is_str || b->is_str) {
        if (!a->is_str || !b->is_str) { cmp_flag = 1; return; }
        int c = strcmp(a->str, b->str);
        if (c < 0) cmp_flag = -1;
        else if (c > 0) cmp_flag = 1;
        else cmp_flag = 0;
    } else {
        long x = a->num;
        long y = b->num;
        if (x < y) cmp_flag = -1;
        else if (x > y) cmp_flag = 1;
        else cmp_flag = 0;
    }
}

void execute_instr(Instr *in) {
    if (!in->op) return;
    if (strcmp(in->op,";")==0) return; // comment line
    if (strcmp(in->op,"HALT")==0) exit(0);
    if (strcmp(in->op,"MOV")==0) {
        char *dst = in->a;
        char *src = in->b;
        Value *r = reg_ptr(dst);
        if (r->is_str && r->str) { free(r->str); r->str = NULL; }
        if (src[0]=='\"') {
            char *s = strdup(src);
            if (s[0]=='\"') {
                size_t L = strlen(s);
                if (L>=2 && s[L-1]=='\"') { s[L-1]='\0'; memmove(s, s+1, strlen(s)); }
            }
            r->is_str = 1; r->str = s; r->num = 0;
        } else {
            if (is_register(src)) {
                Value *rs = reg_ptr(src);
                if (rs->is_str) { r->is_str = 1; r->str = strdup(rs->str); r->num = 0; }
                else { r->is_str = 0; r->num = rs->num; }
            } else {
                r->is_str = 0;
                r->num = atol(src);
            }
        }
        return;
    }
    if (strcmp(in->op,"PUSH")==0) {
        char *arg = in->a;
        Value v;
        if (is_register(arg)) {
            Value *r = reg_ptr(arg);
            if (r->is_str) v = make_str(r->str? r->str : ""); else v = make_num(r->num);
        } else {
            if (arg[0]=='\"') {
                char tmp[MAX_LINE]; strncpy(tmp, arg+1, MAX_LINE-1); tmp[MAX_LINE-1]=0;
                size_t L = strlen(tmp);
                if (L>0 && tmp[L-1]=='\"') tmp[L-1]='\0';
                v = make_str(tmp);
            } else {
                v = make_num(atol(arg));
            }
        }
        push_val(v);
        return;
    }
    if (strcmp(in->op,"POP")==0) {
        char *arg = in->a;
        Value v = pop_val();
        Value *r = reg_ptr(arg);
        if (r->is_str && r->str) { free(r->str); r->str = NULL; }
        r->is_str = v.is_str;
        if (v.is_str) r->str = strdup(v.str? v.str : ""); else r->num = v.num;
        if (v.is_str && v.str) free(v.str);
        return;
    }
    if (strcmp(in->op,"LOAD")==0) {
        char *dst = in->a;
        char *addr = in->b;
        int slot = atoi(addr+1);
        Value *r = reg_ptr(dst);
        if (mem[slot].is_str) {
            if (r->is_str && r->str) free(r->str);
            r->is_str = 1; r->str = mem[slot].str ? strdup(mem[slot].str) : strdup("");
        } else {
            r->is_str = 0; r->num = mem[slot].num;
        }
        return;
    }
    if (strcmp(in->op,"STORE")==0) {
        char *src = in->a;
        char *addr = in->b;
        int slot = atoi(addr+1);
        Value *r = reg_ptr(src);
        if (mem[slot].is_str && mem[slot].str) { free(mem[slot].str); mem[slot].str = NULL; }
        if (r->is_str) { mem[slot].is_str = 1; mem[slot].str = strdup(r->str? r->str : ""); }
        else { mem[slot].is_str = 0; mem[slot].num = r->num; }
        return;
    }
    if (strcmp(in->op,"ADD")==0) {
        Value *a = reg_ptr(in->a);
        Value *b = reg_ptr(in->b);
        if (!a->is_str && !b->is_str) a->num = a->num + b->num;
        else {
            size_t L = strlen(a->str? a->str:"") + strlen(b->str?b->str:"") + 1;
            char *s = malloc(L);
            s[0]='\0';
            if (a->str) strcat(s, a->str);
            if (b->str) strcat(s, b->str);
            if (a->is_str && a->str) free(a->str);
            a->is_str = 1; a->str = s;
        }
        return;
    }
    if (strcmp(in->op,"SUB")==0) { Value *a = reg_ptr(in->a); Value *b = reg_ptr(in->b); a->num = a->num - b->num; return; }
    if (strcmp(in->op,"MUL")==0) { Value *a = reg_ptr(in->a); Value *b = reg_ptr(in->b); a->num = a->num * b->num; return; }
    if (strcmp(in->op,"DIV")==0) { Value *a = reg_ptr(in->a); Value *b = reg_ptr(in->b); if (b->num==0) { fprintf(stderr, "Div by zero\n"); exit(1);} a->num = a->num / b->num; return; }
    if (strcmp(in->op,"CMP")==0) {
        Value *a = reg_ptr(in->a); Value *b = reg_ptr(in->b);
        set_cmp(a,b);
        return;
    }
    if (strcmp(in->op,"JZ")==0) {
        int label = atoi(in->a+1);
        if (cmp_flag == 0) {
            for (int i=0;i<prog_len;i++) {
                if (prog[i].op && prog[i].op[0]=='L') {
                    int lab = atoi(prog[i].op+1);
                    if (lab == label) { pc = i; return; }
                }
            }
        }
        return;
    }
    if (strcmp(in->op,"JNZ")==0) {
        int label = atoi(in->a+1);
        if (cmp_flag != 0) {
            for (int i=0;i<prog_len;i++) {
                if (prog[i].op && prog[i].op[0]=='L') {
                    int lab = atoi(prog[i].op+1);
                    if (lab == label) { pc = i; return; }
                }
            }
        }
        return;
    }
    if (strcmp(in->op,"JMP")==0) {
        int label = atoi(in->a+1);
        for (int i=0;i<prog_len;i++) {
            if (prog[i].op && prog[i].op[0]=='L') {
                int lab = atoi(prog[i].op+1);
                if (lab == label) { pc = i; return; }
            }
        }
        return;
    }
    if (strcmp(in->op,"JLT")==0) {
        int label = atoi(in->a+1);
        if (cmp_flag < 0) {
            for (int i=0;i<prog_len;i++) {
                if (prog[i].op && prog[i].op[0]=='L') {
                    int lab = atoi(prog[i].op+1);
                    if (lab == label) { pc = i; return; }
                }
            }
        }
        return;
    }
    if (strcmp(in->op,"JGT")==0) {
        int label = atoi(in->a+1);
        if (cmp_flag > 0) {
            for (int i=0;i<prog_len;i++) {
                if (prog[i].op && prog[i].op[0]=='L') {
                    int lab = atoi(prog[i].op+1);
                    if (lab == label) { pc = i; return; }
                }
            }
        }
        return;
    }
    if (strcmp(in->op,"JLE")==0) {
        int label = atoi(in->a+1);
        if (cmp_flag <= 0) {
            for (int i=0;i<prog_len;i++) {
                if (prog[i].op && prog[i].op[0]=='L') {
                    int lab = atoi(prog[i].op+1);
                    if (lab == label) { pc = i; return; }
                }
            }
        }
        return;
    }
    if (strcmp(in->op,"JGE")==0) {
        int label = atoi(in->a+1);
        if (cmp_flag >= 0) {
            for (int i=0;i<prog_len;i++) {
                if (prog[i].op && prog[i].op[0]=='L') {
                    int lab = atoi(prog[i].op+1);
                    if (lab == label) { pc = i; return; }
                }
            }
        }
        return;
    }
    if (strcmp(in->op,"READ_SENSOR")==0) {
        char *reg = in->a;
        char *sname = in->b;
        Value *r = reg_ptr(reg);
        if (r->is_str && r->str) { free(r->str); r->str = NULL; }
        if (strcmp(sname,"TEMPO")==0) { r->is_str = 0; r->num = sensor_tempo; }
        else if (strcmp(sname,"DISTANCIA")==0) { r->is_str = 0; r->num = sensor_distancia; }
        else if (strcmp(sname,"BATAMENTOS")==0) { r->is_str = 0; r->num = sensor_batimentos; }
        return;
    }
    if (strcmp(in->op,"PRINT")==0) {
        char *arg = in->a;
        if (is_register(arg)) {
            Value *r = reg_ptr(arg);
            if (r->is_str) printf("%s\n", r->str? r->str : "");
            else printf("%ld\n", r->num);
        } else {
            if (arg[0]=='\"') {
                char tmp[MAX_LINE]; strncpy(tmp, arg+1, MAX_LINE-1); tmp[MAX_LINE-1]=0;
                size_t L = strlen(tmp);
                if (L>0 && tmp[L-1]=='\"') tmp[L-1]='\0';
                printf("%s\n", tmp);
            } else {
                printf("%s\n", arg);
            }
        }
        return;
    }
    if (strcmp(in->op,"WAIT")==0) {
        char *arg = in->a;
        long secs = atol(arg);
        printf("[VM] waiting %ld seconds (simulated)\n", secs);
        return;
    }
    if (in->op[0]=='L') return;
    fprintf(stderr, "Unknown instr: %s %s %s\n", in->op, in->a?in->a:"", in->b?in->b:"");
}

void parse_line(char *line) {
    char *s = trim(line);
    if (s[0]==';' || s[0]=='\0') return;
    if (s[0]=='L') {
        char *c = strchr(s,':');
        if (c) {
            *c = 0;
            prog[prog_len].op = strdup(s);
            prog[prog_len].a = NULL; prog[prog_len].b = NULL;
            prog_len++;
            return;
        }
    }
    char op[MAX_LINE]; char a[MAX_LINE]; char b[MAX_LINE];
    op[0]=a[0]=b[0]=0;
    char *p = s;
    int i=0;
    while(*p && !isspace((unsigned char)*p) && *p!=',') { op[i++]=*p++; }
    op[i]=0;
    while(*p && isspace((unsigned char)*p)) p++;
    if (*p) {
        if (*p=='\"') {
            char *q = strchr(p+1,'\"');
            if (q) {
                size_t L = q - p + 1;
                strncpy(a, p, L); a[L]=0;
                p = q+1;
            } else { strcpy(a, p); p += strlen(p); }
        } else {
            i=0;
            while(*p && !isspace((unsigned char)*p) && *p!=',') { a[i++]=*p++; }
            a[i]=0;
        }
    }
    while(*p && (isspace((unsigned char)*p) || *p==',')) p++;
    if (*p) {
        if (*p=='\"') {
            char *q = strchr(p+1,'\"');
            if (q) {
                size_t L = q - p + 1;
                strncpy(b, p, L); b[L]=0;
                p = q+1;
            } else { strcpy(b, p); p += strlen(p); }
        } else {
            i=0;
            while(*p && !isspace((unsigned char)*p) && *p!=',') { b[i++]=*p++; }
            b[i]=0;
        }
    }
    prog[prog_len].op = strdup(op);
    prog[prog_len].a = strlen(a)? strdup(a): NULL;
    prog[prog_len].b = strlen(b)? strdup(b): NULL;
    prog_len++;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Usage: %s program.asm\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1],"r");
    if (!f) { perror("fopen"); return 1; }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        parse_line(line);
    }
    fclose(f);
    for (pc = 0; pc < prog_len; pc++) {
        Instr *in = &prog[pc];
        execute_instr(in);
    }
    return 0;
}
