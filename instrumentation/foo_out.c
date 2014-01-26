#define L_AND(a, b) a && b
#define L_OR(a, b) a || b

#include<stdio.h>


int doit(int a){
    int b;
    printme(&b);
    
    int c[10];
    printme(&c);
    

    printf("hello!");
}

int main(void){

    int a[20];
    printme(&a);
    
    doit();
    return 0;
}
