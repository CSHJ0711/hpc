#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 12

float **malloc_2d(int row, int col)
{
   float **A, *ptr;
   int len, i;

   len = sizeof(float *)*row + sizeof(float)*col*row;
   A = (float **)malloc(len);
   ptr = (float *)(A + row);
   for ( i = 0 ; i < row; i++)
      A[i] = (ptr + col*i);
   return A;
}

main(int argc, char *argv[])
{
   float A[N][N], B[N][N], C[N][N];
   float **local_A, **local_B, **local_C;
   int local_N, *displs, *counts, i,j,n;
   int np2, np, pid;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np2);

   np = sqrt(np2);
   local_N = N/np;

   local_A = malloc_2d(local_N, local_N);
   local_B = malloc_2d(local_N, local_N);
   local_C = malloc_2d(local_N, local_N);

   //initialize of A and B
   
   if (pid == 0){
      for ( i = 0; i < N; i++)
         for ( j = 0; j < N; j++){
            A[i][j] = i*N+j;
            B[i][j] = N*N-i*N-j;
            printf("A[%d][%d] :: %f\n",i,j,A[i][j]);
            printf("B[%d][%d] :: %f\n",i,j,B[i][j]);
         }
      }

   // (i) decompose A and B into local_A and local_B
   displs = (int*)malloc(sizeof(int)*np2);
   counts = (int*)malloc(sizeof(int)*np2);
   
   for ( i = 0; i < np2; i++)
   {
      counts[i] = local_N * local_N;
      displs[i] = i * local_N * local_N;
   }
   
   MPI_Scatterv(A,counts,displs,MPI_FLOAT,local_A[0],local_N*local_N,MPI_FLOAT,0,MPI_COMM_WORLD);
   MPI_Scatterv(B,counts,displs,MPI_FLOAT,local_A[0],local_N*local_N,MPI_FLOAT,0,MPI_COMM_WORLD);

   //(ii) local_C = local_A + local_B
   
   for ( i = 0; i < local_N; i++) 
      for ( j = 0; j < local_N; j++)
      {
         local_C[i][j] = local_A[i][j] + local_B[i][j];
         printf("local_C[%d][%d] :: %f\n",i,j,local_C);
      }

   //(iii) compose local_C to C
   
   MPI_Gatherv(local_C[0],local_N * local_N,MPI_FLOAT,C,counts,displs,MPI_FLOAT,0,MPI_COMM_WORLD);
   
   //check the results
   if (pid == 0)
      for ( i = 0; i < N; i++) {
         for ( j = 0; j < N; j++)
            printf("%3.0f",C[i][j]);
         printf("\n");
      }
  
   free(local_A);
   free(local_B);
   free(local_C);
   free(displs);
   free(counts);

   MPI_Finalize();
}
