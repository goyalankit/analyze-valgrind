#include "/work/02681/ankitg/workspace/valgrind/gleipnir/gleipnir.h"
#include<stdio.h>

void foo(int a[10], int b[10], int c[100]){
    GL_GLOBAL_START_INSTR;
    int i;
    for(i=9; i > 0; i--){ 
        b[i] = 12;
        if(i-1 > 1)
            a[i] = 21 + b[i-1];
    }
    
    for(i=99; i>0; i--){
        c[i] = b[i%10];
    }

    b[3]  = 23;
    GL_GLOBAL_STOP_INSTR;
}

int main(void){
    int a[10];
    printf("varinfo:a:10:%08lx\n",(void *) &a);
    int b[10];
    int c[100];
    printf("varinfo:b:10:%08lx\n",(void *) &b);
    printf("varinfo:c:100:%08lx\n",(void *) &c);
    a[2] = 23;
    printf("%d", a[2]);

  //  GL_GLOBAL_START_INSTR;
    foo(a, b, c);
  //  GL_GLOBAL_STOP_INSTR;

    return 0;
}
