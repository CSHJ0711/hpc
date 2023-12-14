#define N 13
int printf(char *, ...);
int f(int i)
{
   i = i * 2;
   return i;
}

main()
{
   int A[N], B[N], i;
   omp_set_num_threads(4);
   A[0] = f(0);
   B[0] = f(0);
   #pragma omp parallel for
   for (i = 1; i < N; i++) {
      A[i] = f(i);
   #pragma omp parallel for
   for (i = 1; i < N; i++)
      B[i] = A[i-1] + A[i];
   }
   for (i = 0; i < N; i++)
      printf("%d: %d %d\n",i,A[i],B[i]);
}
