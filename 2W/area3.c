#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double f(double x) {
  double denominator = (x * x + 2) * (x * x + 2) * (x * x + 2);
  return x / denominator;
}

main(int argc, char *argv[])
{
  long N; //user argument input N
  int i; //for loop
  double a = 0.0, b = 2.0; //range of x 0.0 to 2.0
  double sum = 0.0;

  if (argc != 2) { //is there 2 arguments?
    printf("argument error\n");
    exit(1);
  }
  N = atoi(argv[1]);
  double dx = (b-a)/N;

  double x = a;
  for (i = 0; i < N; i++){
  sum += 0.5 * (f(x) + f(x + dx)) * dx;
  x += dx;
  }
  printf("result :: %lf\n",sum);
  return 0;
}
