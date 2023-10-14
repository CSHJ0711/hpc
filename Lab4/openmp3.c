#define N 12
int printf(char *,...);

main()
{
  int tid,A[N], i;
  omp_set_num_threads(4);

  #pragma omp parallel for
  for ( i = 0; i < N; i++)
    A[i] = N-i-1;

  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    #pragma omp for
    for ( i = 0; i < N; i++)
      printf("%d %d %d\n", tid, i, A[i]);
  }
}
