/* author: cxy<caixiangyue007@gmail.com> */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define X_VERSION 0.1

int token; // current token
char *src, *old_src;
int pool_size = 256 << 10;
int line;
int *text, *old_text, *stack;
char *data;

// register
int *pc, *bp, *sp, ax, cycle;

// instructions
enum { LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
       OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT };

int eval()
{
    int op, *tmp;
    while(1){
        op = *pc++;
        if (op == IMM) {ax = *pc++;}
        else if (op == LC) {ax = *(char*)ax;} // ax store pointer
        else if (op == PUSH) {*--sp = ax;}
        else if (op == ADD) {ax = *sp++ + ax;}
        else if (op == EXIT) {printf("EXIT(%d)\n", *sp); return *sp;}
    }
    return 0;
}

void next()
{
    token = *src++;
    return;
}

void expression(int level)
{

}

void program()
{
    next();                  // get next token
    while (token > 0) {
        printf("token is: %c\n", token);
        next();
    }
}

int main(int argc, char **argv)
{
    int i;
    FILE *fd;
    line = 1;
    argv++;
    if ((fd = fopen(*argv, "r")) == NULL) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    if (!(src = old_src = malloc(pool_size))) {
        printf("could not malloc(%d) for source area\n", pool_size);
        return -1;
    }

    // read the source file

    if ((i = fread(src, sizeof(char), pool_size - 1, fd)) < 0) {
        printf("read() returned %d\n", i);
        return -1;
    }

    src[i] = 0; // add EOF character
    fclose(fd);

    // allocate memory for virtual machine
    if (!(text = old_text = malloc(sizeof(int) * pool_size))) {
        printf("could not malloc(%d) for text area\n", pool_size);
        return -1;
    }
    if (!(data = malloc(sizeof(int) * pool_size))) {
        printf("could not malloc(%d) for data area\n", pool_size);
        return -1;
    }
    if (!(stack = malloc(sizeof(int) * pool_size))) {
        printf("could not malloc(%d) for stack area\n", pool_size);
        return -1;
    }

    memset(text, 0, pool_size);
    memset(data, 0, pool_size);
    memset(stack, 0, pool_size);

    bp = sp = (stack + pool_size);
    ax = 0;

    i = 0;
    text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc = text;

    program();
    eval();
    if (old_src)
        free(old_src);

    free(text);
    free(data);
    free(stack);

    return 0;

}
