#include <stdio.h>
#include "mpi.h"

#define N 24

main(int argc, char* argv[])
{
   int np, pid, i, tag = 0,n;
   float A[N], B[N], C[N], local_A[N], local_B[N], local_C[N];
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   //initialize of A and B
   if (pid == 0)
   {
      for ( i = 0; i < N; i++)
      {
         A[i] = i;
         B[i] = N-i;
      }
   }
   //(i) decomposition
   n = N/np;
   MPI_Scatter(A,n,MPI_FLOAT,local_A,n,MPI_FLOAT,0,MPI_COMM_WORLD);
   MPI_Scatter(B,n,MPI_FLOAT,local_B,n,MPI_FLOAT,0,MPI_COMM_WORLD);
   //(ii) addition
   for ( i = 0; i < N/np; i++)
      local_C[i] = local_A[i] + local_B[i];

   //(iii) composition
   MPI_Gather(local_C,n,MPI_FLOAT,C,n,MPI_FLOAT,0,MPI_COMM_WORLD);
   //print results
   if(pid == 0)
   {
      for ( i = 0; i < N; i++)
         printf("%2.1f",C[i]);
      printf("\n");
   }
  
   MPI_Finalize();
}
