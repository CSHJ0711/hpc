#define N 12
int printf(char *, ...);

main()
{
   int tid, A[N], sum, i;
   omp_set_num_threads(4);

   #pragma omp parallel for
   for(i = 0; i < N; i++)
      A[i] = i * 2;

   sum = 0;
   #pragma omp parallel private(tid)
   {
      int local_sum = 0;
      #pragma omp for
      for (i = 0; i < N; i++) {
         tid = omp_get_thread_num();
         local_sum = local_sum + A[i];
         printf("%d %d %d %d\n",tid,i,A[i],local_sum);
      }
      #pragma omp critical
      sum += local_sum;
   }
   printf("%d\n", sum);
   return 0;
}
