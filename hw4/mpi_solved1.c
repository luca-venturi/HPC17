/******************************************************************************
* FILE: mpi_bug1.c
* Comment: the 'tag' variables were not set correctly. I changed them so that 'tag' = 'destinary of the message'. Also included 'return 0' at the end of 'main' and removed variabled 'rc', which was useless, in order to avoid warning messages in compiling.  
******************************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
int numtasks, rank, dest, tag, source, count;
char inmsg, outmsg='x';
MPI_Status Stat;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
printf("Task %d starting...\n",rank);

if (rank == 0) {
  if (numtasks > 2) 
    printf("Numtasks=%d. Only 2 needed. Ignoring extra...\n",numtasks);
  dest = rank + 1;
  source = dest;
  tag = rank;
  MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag + 1, MPI_COMM_WORLD);
  printf("Sent to task %d...\n",dest);
  MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
  printf("Received from task %d...\n",source);
  }

else if (rank == 1) {
  dest = rank - 1;
  source = dest;
  tag = rank;
  MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
  printf("Received from task %d...\n",source);
  MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag - 1, MPI_COMM_WORLD);
  printf("Sent to task %d...\n",dest);
  }

if (rank < 2) {
  MPI_Get_count(&Stat, MPI_CHAR, &count);
  printf("Task %d: Received %d char(s) from task %d with tag %d \n",
         rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
  }

MPI_Finalize();

return 0;
}
