int printf(char *,...);

main()
{
  int nt, tid;
  omp_set_num_threads(4);
  #pragma omp parallel
  {
    nt = omp_get_num_threads();
    tid = omp_get_thread_num();
    printf("%d %d\n",nt, tid);
  }
}
