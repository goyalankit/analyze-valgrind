#define L_AND(a, b) a && b
#define L_OR(a, b) a || b

#include<stdio.h>


int doit(int a){
    int b;
    int c[10];
    printf("varinfo:c:10:%08lx\n",(void *) &c);
    
    printf("hello!");
}

int main(void){

    int a[20];
    printf("varinfo:a:20:%08lx\n",(void *) &a);
    doit(12);
    return 0;
}
