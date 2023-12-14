#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

long millisecond()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return(1000*tv.tv_sec + tv.tv_usec/1000) ;
}

double f(double x)
{
   double temp;
   temp = x * x + 2;
   return x / (temp * temp * temp);
}

int main(int argc, char *argv[])
{
   long N, i;
   double a, b, area, local_area, dx, x;
   long start, elapsed;
   int rank, size, source;

   if (argc != 2) {
      if (rank == 0) {
         printf("Usage: %s N\n", argv[0]);
      }
      MPI_Finalize();
      exit(1);
   }

   N = atol(argv[1]);

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   start = millisecond();
   a = 0.0;
   b = 1.0;
   dx = (b - a) / (double)N;

   local_area = 0.0;
   for (i = rank; i < N; i += size) {
      x = a + i * dx;
      local_area += 0.5 * (f(x) + f(x + dx)) * dx;
   }

   if (rank != 0) {
      MPI_Send(&local_area, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
   } else {
      area = local_area;
      for (source = 1; source < size; source++) {
         MPI_Recv(&local_area, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         area += local_area;
      }
      elapsed = millisecond() - start;
      printf("\narea(MPI): %5.10f\n", area);
      printf("elapsed time(MPI): %ld milliseconds\n", elapsed);
      printf("GFLOPS(MPI): %5.2f\n", (N * 12.0 / (elapsed / 1000.0) / 1000000000.0));
   }

   MPI_Finalize();
   return 0;
}
