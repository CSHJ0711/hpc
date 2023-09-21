#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double f(double x) {
  return 1.0 / (2.0 * x * x * x); // 1/2z^3
}

main(int argc, char *argv[])
{
  long N; //user argument input N
  int i; //for loop
  double a = 2.0, b = 6.0; //range of x 0.0 to 2.0
  double sum = 0.0;
  double dx, x;

  if (argc != 2) { //is there 1 arguments?
    printf("argument error\n");
    exit(1);
  }
  N = atol(argv[1]); //char to long
  
  dx = (b-a)/N;

  x = a;
  for (i = 0; i < N; i++){
    sum += 0.5 * (f(x) + f(x + dx)) * dx;
    x += dx; 
  }
  printf("result :: %lf\n",sum);
  return 0;
}
