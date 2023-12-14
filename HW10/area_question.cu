#include <stdio.h>
#include <math.h>

#define N 1280000
#define THREADS 128

__device__ float f(float x)
{
   //1) complelte
}

__global__ void area_kernel(float *sums)
{
   float a, b, dx, x, y;
   int i = blockDim.x * blockIdx.x + threadIdx.x;
   __shared__ float sdata[THREADS];

   //2) complelte
   sdata[threadIdx.x] = ...;

}

int main()
{
   float *sums, *sums_d, area, elapsed;
   int i;

   cudaSetDevice(0);

   dim3 dimBlock(THREADS);
   dim3 dimGrid((N + dimBlock.x-1)/dimBlock.x);


   cudaEvent_t start, stop;
   cudaMalloc((void **)&sums_d, sizeof(float)*dimGrid.x);
   sums = (float *)malloc(sizeof(float)*dimGrid.x);

   //3) COMPLETE (timeing - start)

   //4) Complete (call GPU function)
   area_kernel<<<dimGrid, dimBlock>>>(sums_d);

   //copy values from GPU memovry to CPU memory
   cudaMemcpy(sums,sums_d,sizeof(float)*dimGrid.x,cudaMemcpyDeviceToHost);

  //5) Complete (timeing - stop)

   //add the computed value to the value of pi
   area = 0.0;
   for ( i = 0 ; i < dimGrid.x; i++)
      area += sums[i];

   printf("area : %5.10f\n",area);
   printf("elpased time: %f milliseconds\n",elapsed);
   printf("GFLOPS : %5.2f\n", (N*16.0/(elapsed/1000.0))/1000000000.0);

   cudaFree(sums_d);
   free(sums);
}
