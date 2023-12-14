#include <stdio.h>

#define N 1280000
#define THREADS 128

__device__ float f(float x)
{
   double temp;
   temp = x*x+2;
   return x/(temp*temp*temp);
}

__global__ void area_kernel(float *sums)
{
   float a, b, dx, x;
   int i = blockDim.x * blockIdx.x + threadIdx.x;
   int j;
   __shared__ float sdata[THREADS];

   //2) complelte
   a = 0.0;
   b = 2.0;
   dx = (b - a) / N;

   x = a + i * dx;
   sdata[threadIdx.x] = 0.5 * (f(x) + f(x+dx)) * dx;;
   __syncthreads();

   for (j = blockDim.x / 2; j > 0; j >>= 1)
   {
      if (threadIdx.x < j)
      {
         sdata[threadIdx.x] += sdata[threadIdx.x + j];
      }
      __syncthreads();
   }
   if(threadIdx.x == 0)
   {
      sums[blockIdx.x] = sdata[0];
   }

}

int main()
{
   float *sums, *sums_d, area, elapsed;
   int i;

   cudaSetDevice(0);

   dim3 dimBlock(THREADS);
   dim3 dimGrid((N + dimBlock.x-1)/dimBlock.x);

   printf("dimBlock.x :: %d\n",dimBlock.x);
   printf("dimBlock.y :: %d\n",dimBlock.y);
   printf("dimGrid :: %d\n",dimGrid.y);

   cudaEvent_t start, stop;

   cudaMalloc((void **)&sums_d, sizeof(float)*dimGrid.x);
   sums = (float *)malloc(sizeof(float)*dimGrid.x);

   //3) COMPLETE (timeing - start)
   cudaEventCreate(&start);
   cudaEventCreate(&stop);
   cudaEventRecord(start,0);

   //4) Complete (call GPU function)
   area_kernel<<<dimGrid, dimBlock>>>(sums_d);

   //copy values from GPU memovry to CPU memory
   cudaMemcpy(sums,sums_d,sizeof(float)*dimGrid.x,cudaMemcpyDeviceToHost);

   //5) Complete (timeing - stop)
   cudaEventRecord(stop,0);
   cudaEventSynchronize(stop);
   cudaEventElapsedTime(&elapsed,start,stop);

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
