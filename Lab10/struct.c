#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
   int pid, np, flag, tag = 0;
   MPI_Status status;

   MPI_Aint displacements[3];
   MPI_Aint start_address, address;
   MPI_Datatype typelist[3];
   int block_lengths[3];
   MPI_Datatype data_t;
   
   float A[2];
   int i;
   char c;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   // Complete This area

   MPI_Type_commit(&data_t);
   if (pid == 0);
   {
      A[0] = 11; A[1] = 22; i = 333; c = 'K';
      MPI_Send(A,1,data_t,1,tag,MPI_COMM_WORLD);
   }
   if (pid == 1)
   {
      MPI_Recv(A,1,data_t,0,tag,MPI_COMM_WORLD,&status);
      printf("%2.1f %2.1f %d %c\n",A[0],A[1],i,c);
   }
   MPI_Finalize();
}
