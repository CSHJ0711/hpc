#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

main(int argc, char* argv[])
{
   int np,pid,inp,jnp,i,j,k,*buf;
   int dim_sizes[2],wrap_around[2];
   int direct,shift,source,dest,reorder,tag = 0;
   MPI_Comm grid_comm;
   MPI_Status status;

   int coordinates[2]; //i and j location in the grid
   MPI_Group diag_world;
   MPI_Group diag_group;
   MPI_Comm diag_comm;
   int *diag_rank;
   int data;

   MPI_Init(&argc, &argv);
   
   MPI_Comm_rank(MPI_COMM_WORLD,&pid);
   MPI_Comm_size(MPI_COMM_WORLD,&np);

   inp = sqrt(np);jnp = np / inp;
   dim_sizes[0] = inp;
   dim_sizes[1] = jnp;
   wrap_around[0] = wrap_around[1] = 1;

   MPI_Cart_create(MPI_COMM_WORLD,2,dim_sizes,wrap_around,reorder,&grid_comm);
   MPI_Cart_coords(grid_comm,pid,2,coordinates);

   //new communicator and brodcast
   diag_rank = (int*)malloc(jnp*sizeof(int));
   for ( i = 0; i < jnp; i++)
      diag_rank[i] = i * inp + (inp - 1);

   MPI_Comm_group(grid_comm,&diag_world);
   MPI_Group_incl(diag_world,jnp,diag_rank,&diag_group);
   MPI_Comm_create(grid_comm,diag_group,&diag_comm);

   data = pid + 10;
   if (coordinates[0] == jnp - 1)
      MPI_Bcast(&data,1,MPI_INT,jnp-1,diag_comm);

   ///print the shifted matrix
   if (pid == 0) buf = (int *)malloc(sizeof(int)*np);
   MPI_Gather(&data,1,MPI_INT,buf,1,MPI_INT,0,MPI_COMM_WORLD);
   if (pid == 0)
   {
      k = 0;
      for ( i = 0; i < inp; i++)
      {
         for ( j = 0; j < jnp; j++)
            printf("%2d ",buf[k++]);
         printf("\n");
      }
      free(buf);
   }
   MPI_Group_free(&diag_world);
   MPI_Group_free(&diag_group);
   MPI_Comm_free(&diag_comm);
   MPI_Comm_free(&grid_comm);

   MPI_Finalize();
}

