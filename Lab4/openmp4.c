#define N 8
int printf(char *,...);

main()
{
  int tid,A[N], i;
  omp_set_num_threads(5); //4

  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    #pragma omp for
    for ( i = 0; i < N; i++)
      printf("%d %d\n", tid, i);
  }
}
