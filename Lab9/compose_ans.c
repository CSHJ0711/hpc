#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 24

int **malloc_2d(int row, int col)
{
   int **A, *ptr;
   int len, i;

   len = sizeof(float *)*row + sizeof(float)*col*row;
   A = (int **)malloc(len);
   ptr = (int *)(A + row);
   for ( i = 0; i < row; i++)
      A[i] = (ptr + col * i);
   return A;
}

main(int argc, char *argv[])
{
   int A[N][N], **local_A;
   int local_N, i, j, source;
   int np2, np, pid;
   MPI_Status status;
   int tag;
   int row, col;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np2);

   np = sqrt(np2);
   local_N = N/np;

   local_A = malloc_2d(local_N, local_N);

   //initialize of arrays
   if (pid == 0) {
      for ( i = 0; i < local_N; i++)
         for ( j = 0; j < local_N; j++)
            A[i][j] = pid;
   }
   else {
      for ( i = 0; i < local_N; i++)
         for ( j = 0; j < local_N; j++)
            local_A[i][j] = pid;
   }
   //composition
   for ( i = 0; i < local_N; i++)
   {
      if (pid != 0)
      {
         MPI_Send(&local_A[i][0], local_N, MPI_INT, 0, tag, MPI_COMM_WORLD);
      }
      else
         for ( source = 1 ; source < np2; source++)
         {
            x = (source / np) * local_N;
            y = (source % np)*local_N;
            MPI_Recv(&A[x+i][y],local_N, MPI_INT,source,tag,MPI_COMM_WORLD,&status);
         }
   //print the result
   if (pid == 0)
      for ( i = 0; i < N; i++) {
         for (j = 0; j < N; j++)
            printf("%3d",A[i][j]);
         printf("\n");
      }
   MPI_Finalize();
}
