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
   temp = x*x+2;
   return x/(temp*temp*temp);
}

main(int argc, char *argv[])
{
   long N, i;
   double a, b, area, dx, x,temp;
   long start, elapsed;
   long local_N;
   int pid, np, source,dest, tag = 0;

   MPI_Status status;
   
   if (argc != 2){
      printf("argument error\n");
      exit(1);
   }

   N = atol(argv[1]);

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   local_N = N/(long)np;

   a = 0.0, b = 2.0;
   dx = (b-a) / (double)N;

   // parallel(MPI) calculation of area

   area = 0.0;

   start = millisecond();

   x = ((b-a)/(double)np)*(double)pid; 
   for ( i = 0; i < local_N; i++) {
      area += 0.5 * (f(x) + f(x+dx)) * dx;
      x += dx;
   }
 
   if(pid != 0) { // p1 ~
      MPI_Send(&area,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
   }

   else { //p0
      for(source = 1;source < np;source++) {
         MPI_Recv(&temp, 1, MPI_DOUBLE,source, tag, MPI_COMM_WORLD, &status);
         area += temp;
      }
   }
   elapsed = millisecond() - start;

   if (pid == 0) {
      printf("\narea(MPI): %5.10f\n", area);
      printf("elapsed time(MPI : %ld milliseconds\n", elapsed);  
      printf("GFLOPS(MPI): %5.2f\n", (N*21.0 / (elapsed / 1000.0)) / 1000000000.0);
   }
   MPI_Finalize();
   exit(0);
}
