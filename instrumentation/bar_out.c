#define L_AND(a, b) a && b
#define L_OR(a, b) a || b

#include<stdio.h>

void foo(int b[100], int a[100]){
    int i;
    for(i=99; i > 0; i--){ 
        b[i] = 12;
        if(i-1 > 1)
            a[i] = 21 + b[i-1];
    }
    b[3]  = 23;
}

int main(void){
    int a[100];
    fprintf(stderr,"varinfo:a:100:%08lx\n",(void *) &a);
    int b[100];
    fprintf(stderr,"varinfo:b:100:%08lx\n",(void *) &b);
    a[2] = 23;
    printf("%d", a[2]);
    
    fprintf(stderr,"\nbeforemethodcall:foo\n");
    foo(a, b);
    fprintf(stderr,"\naftermethodcall:foo\n");
    

    return 0;
}
