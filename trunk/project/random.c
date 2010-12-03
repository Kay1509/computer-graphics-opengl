#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {

int random,i;

for(i =0;i<100;i++)
fprintf(stderr," %d", random=(int)(8*(rand()/(RAND_MAX+1.0))));
fprintf(stderr,"\n\n"); 
for(i =0;i<100;i++)
fprintf(stderr," %d", random=(int)(rand()%8));
}