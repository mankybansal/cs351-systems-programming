#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <setjmp.h>

jmp_buf buf;

void f1 (int x) {
    printf("In f1\n");
    f2(x);
}

void f2 (int x) {
    printf("In f2\n");
    if (!x)
        longjmp(buf, 1);
}

int main () {
    int i = 0;
    if (setjmp(buf) != 0) {
        printf("Restarting...\n");
    }
    f1(i++);
    printf("Exiting.\n");
    return 0;
}