#include <stdio.h>
#include "mpi.h"

main(int argc, char* argv[])
{
   int np, pid, tag = 0;
   float data, local_data;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   local_data = pid + 100.0;
   //fill in this blank
   MPI_Reduce(&local_data, &data, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

   if (pid == 0) 
      printf("%f\n",data);
   MPI_Finalize();
}
