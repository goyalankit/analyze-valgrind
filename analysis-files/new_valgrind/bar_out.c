#include<stdio.h>

void foo(int a[10], int b[10], int c[100]){
    int i;
    for(i=9; i > 0; i--){ 
        b[i] = 12;
        if(i-1 > 1)
            a[i] = 21 + b[i-1];
    }
    
    for(i=99; i>0; i--){
        c[i] = b[i/10-1];
    }

    b[3]  = 23;
}

int main(void){
    int a[10];
    int b[10];
    int c[100];
    a[2] = 23;
    printf("%d", a[2]);

  //  GL_GLOBAL_START_INSTR;
    foo(a, b, c);
  //  GL_GLOBAL_STOP_INSTR;

    return 0;
}
