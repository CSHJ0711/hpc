#include <stdlib.h>
int printf(char *,...);

main(int argc, char *argv[])
{
  int tid, i, N;
  if (argc != 3) {
    printf("usage: %s #threads #size_of_arrays\n",argv[0]);
    exit(1);
  }
  omp_set_num_threads(atoi(argv[1]));
  N = atoi(argv[2]);
  int A[N], B[N], C[N];

  for ( i = 0; i < N; i++) {
    A[i] = i;
    B[i] = N-i;
  }

  #pragma omp parallel for
  for (i = 0; i < N; i++)
    C[i] = A[i] + B[i];

  #pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    #pragma omp for
    for ( i = 0; i < N; i++)
      printf("%d %d %d\n",tid, i, C[i]);
  } 
}
