#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN  100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);

int  gettoken(void);
int  tokentype;          /* type of last token */
char token[MAXTOKEN];    /* last token string */
char name[MAXTOKEN];     /* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */
char out[1000];          /* output string */

int error;

int main(int argc, char *argv[]) {
    while (gettoken() != EOF) {
        strcpy(datatype, token);
        out[0] = '\0';
        dcl();
        if (tokentype != '\n') {
            printf("syntax error\n");
            error = 1;
        } else if (error) {
            error = 0;
        } else {
            printf("%s: %s %s\n", name, out, datatype);
        }
    }
    return 0;
}

/* dcl:  parse a declarator */
void dcl(void) {
    int ns;
    for (ns = 0; gettoken() == '*'; ) {
        ns++;
    }
    dirdcl();
    while (ns-- > 0) {
        strcat(out, " pointer to");
    }
}

/* dirdcl:  parse a direct declarator */
void dirdcl(void) {
    int type;
    if (tokentype == '(') {
        dcl();
        if (tokentype != ')') {
            printf("error; missing )\n");
        }
    } else if (tokentype == NAME) {
        strcpy(name, token);
    } else {
        printf("error: expected name or (dcl)\n");
        error = 1;
        return;
    }
    while ((type=gettoken()) == PARENS || type == BRACKETS) {
        if (type == PARENS) {
            strcat(out, " function returning");
        } else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
}

/* gettoken:  return next token */
int gettoken(void) {
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    if (error) {
        while ((c = getch()) != '\n') { }
        error = 0;
    } 

    while ((c = getch()) == ' ' || c == '\t') { }

    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c == '[') {
        for (*p++ = c; (*p++ = getch()) != ']'; ) { }
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch()); ) {
            *p++ = c;
        }
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else {
        return tokentype = c;
    }
}
