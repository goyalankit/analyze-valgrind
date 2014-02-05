#include<stdio.h>
void main(void){

    int i[10];
    printf("varinfo:i:%d:%08lx\n",sizeof(int),(void *) &i);
    int j = 0;
    for(j=0; j<10; j++){
        i[j]++;
    }
}
