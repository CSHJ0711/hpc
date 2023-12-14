#define N 1000000
#define NT 4

int printf(char *, ...);

main()
{
   double oscillation, sum;
   int i;

   oscillation = 1.0;
   sum = 0.0;
   #pragma omp parallel for num_threads(NT) reduction(+:sum) private(oscillation)
   for(i = 0; i < N; i++) {
      if(i%2 == 0) oscillation = 1.0;
      else oscillation = -1.0;
      sum += oscillation/(2*i+1);
   }
   printf("%f\n", 4.0*sum);
}
