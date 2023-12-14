#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

long millisecond()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return(1000*tv.tv_sec + tv.tv_usec/1000);
}

double f(double x) {
  return x / ((x * x + 2) * (x * x + 2) * (x * x + 2)); //div mul add mul mul add mul mul add (9)
}

main(int argc, char *argv[])
{
  long start, stop, N, i; // program start stop time & user argument input N & for loop
  double a = 0.0, b = 2.0, sum = 0.0, dx, x; //range of x 0.0 to 2.0 (a,b)
  start = millisecond(); //time measure start
  
  if (argc != 2) { //is there 1 arguments?
    printf("argument error\n");
    exit(1);
  }
  N = atol(argv[1]);

/* -------------- floationg point operation count start ------------------ */
  dx = (b-a)/N; // sub, div (2)

  x = a;
  for (i = 0; i < N; i++){
    sum += 0.5 * (f(x) + f(x + dx)) * dx; //add mul add add add mul (6) [call function * 2 = 9 * 2(18)] = (24) 
    x += dx; //add (1)
  }
/*--------------- floating point operation count end -------------------- */
  stop = millisecond(); //time measure stop
  printf("result :: %5.10lf\n",sum);
  printf("elapsed time :: %ld milliseconds\n",stop-start);
  if (stop-start != 0) {
    printf("GFLOPS :: %lf\n", (N*27)/(stop-start) * 1e-9);
//GFLOPS = N * #Floating point operations / elapsed time * 10^-9
  }
  else {
    printf("Unable to calculate GFLOPS (elapsed time = 0)\n");
    exit(1);
  }
  exit(0);
}
