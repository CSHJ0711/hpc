#include <stdio.h>
#include <string.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
   int a,b,c;

   if(argc != 3) exit(1);
   a = atoi(argv[1]);
   b = atoi(argv[2]);
   c = a + b;
   printf("%d\n",c);
   exit(0);
}
