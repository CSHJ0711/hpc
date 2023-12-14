#include <stdio.h>
#include "mpi.h"

#define N 24

main(int argc, char* argv[])
{
   int np, pid, i, tag = 0, local_N;
   float A[N], B[N], C[N];
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
   local_N = N/np;
   if (pid == 0) //p0, send
   {
      for ( i = 1; i < np; i++)
      {
         MPI_Send(A+i*local_N,local_N,MPI_FLOAT,i,tag,MPI_COMM_WORLD);
         MPI_Send(B+i*local_N,local_N,MPI_FLOAT,i,tag,MPI_COMM_WORLD);
      }
   }
   else //p1~, Recv
   {
      MPI_Recv(A, local_N, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(B, local_N, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
   }
    
   //(ii) addition
   for ( i = 0; i < N/np; i++)
      C[i] = A[i] + B[i];

   //(iii) composition
   if (pid != 0) //p1 ~, Send
   {
      MPI_Send(C,local_N, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
   }
   else // p0, Recv
   {
   for ( i = 1; i < np; i++)
      MPI_Recv(C+i*local_N, local_N, MPI_FLOAT, i, tag, MPI_COMM_WORLD, &status);
   }

   //print results
   if(pid == 0)
   {
      for ( i = 0; i < N; i++)
         printf("%2.1f",C[i]);
      printf("\n");
   }
  
   MPI_Finalize();
}
