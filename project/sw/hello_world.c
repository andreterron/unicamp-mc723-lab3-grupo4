#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int i;
  for(i=0;i<10;i++)
    printf("Hi from processor %s!\n", argv[argc-1]);

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

