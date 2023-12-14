#include <stdio.h>
#include "mpi.h"

#define N 24

main(int argc, char* argv[])
{
   int np, pid, i, tag = 0,n;
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
   n = N/np;
   if (pid == 0) //p0, send
   {
      for(dest = 1; dest < np; dest++)
      {
         MPI_Send(A+dest*n,n,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
         MPI_Send(B+dest*n,n,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
      }
   }
   else //p1~, Recv
   {
      MPI_Recv(A,n, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(B,n, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
   }
    
   //(ii) addition
   for ( i = 0; i < N/np; i++)
      C[i] = A[i] + B[i];

   //(iii) composition
   if (pid != 0) //p1 ~, Send
   {
      MPI_Send(C,n, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
   }
   else // p0, Recv
   {
   for (src=1;src<np;src++)
      MPI_Recv(C+src*n, n, MPI_FLOAT, src, tag, MPI_COMM_WORLD, &status);
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
