#include<stdio.h>
void main(void){
    int i[10];
    printf("varinfo:i:10:%08lx\n",(void *) &i);
    int j = 0;
    fprintf(stderr,"start");
    for(j=0; j<10; j++){
        i[j]++;
    }
    fprintf(stderr, "end");
}
