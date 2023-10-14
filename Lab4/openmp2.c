int printf(char *,...);

main()
{
  int tid, sum = 0;
  omp_set_num_threads(4);
  #pragma omp parallel private(tid) shared(sum)
  {
    tid = omp_get_thread_num();
    #pragma omp critical // atomic - this line may not be required
    sum += tid;
  }
  printf("%d\n", sum);
}
