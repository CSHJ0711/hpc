#include <stdio.h>
#include <math.h>

#define N 10000
#define Threads 100

__global__ void saxpy(float *a,float *b,float x,float y)
{
   int i = blockIdx.x * blockDim.x + threadIdx.x;
   if (i < N)
   {
      b[i] = a[i] * x + y;
   }
}
int main()
{
   float A[N],B[N],B2[N],X,Y;
   int i;
   float *A_d, *B_d;

   cudaSetDevice(0);
   
   for ( i = 0; i < N; i++)
      A[i] = i * 2;

   X = 1.23;
   Y = 2.34;
   
   cudaMalloc((void **)&A_d, N * sizeof(float));
   cudaMalloc((void **)&B_d, N * sizeof(float));

   cudaMemcpy(A_d, A, N * sizeof(float),cudaMemcpyHostToDevice);

   dim3 dimBlock(Threads,1,1);
   dim3 dimGrid((N + Threads - 1) / Threads,1,1);

   saxpy<<<dimGrid, dimBlock>>>(A_d,B_d,X,Y);

   cudaMemcpy(B, B_d, N * sizeof(float),cudaMemcpyDeviceToHost);

   cudaFree(A_d);
   cudaFree(B_d);

   for ( i = 0; i < N; i++)
   {
      B2[i] = A[i]*X + Y; //B2 is used for checking
   }

   //check the results
   for ( i = 0; i < N; i++)
      if (fabs(B[i] - B2[i]) > 0.01 )
         printf("%d: %f %f\n",i,B[i],B2[i]);

}
