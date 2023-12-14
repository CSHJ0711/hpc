#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

main(int argc, char *argv[])
{
   long N, local_N, i;
   double sum, x, temp;
   int pid, np, source, dest;
   int tag = 0;
   MPI_Status status;

   if ((argc != 2)) {
      printf("usage: %s N\n", argv[0]);
      exit(1);
   }

   N = atol(argv[1]);

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   local_N = N/np;

   sum = 0.0;
   x = 1.0 + N/(double)np*(double)pid;
   for (i=0; i<local_N; i++) {
      sum += x;
      x += 1.0;
   }

   // FILL IN THIS BLANK
   
   if (pid == 0) {
      for ( source = 1; source < np; source ++) {
         MPI_Recv(&temp, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
         sum += temp;
      }
      printf("sum: %8.1f\n", sum);
   }
   else {
      MPI_Send(&sum, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
   }

   MPI_Finalize();
   exit(0);
}
