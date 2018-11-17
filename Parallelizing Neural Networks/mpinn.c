#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define size 4000                  /* number of columns in matrix B */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

float sigmoid(float x)
{
     float exp_value;
     float return_value;
     exp_value = exp((float) -x);
     return_value = 1 / (1 + exp_value);
     return return_value;

}


int main (int argc, char *argv[])
{
int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  
	averow, extra, offset, /* used to determine rows sent to each worker */
	i, j, k, rc;           /* misc */


float	weight[size][size],          
	layer[size][size],           
	output[size][size];           



float f;

MPI_Status status;

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
if (numtasks < 2 ) {
  printf("Need at least two MPI tasks. Quitting...\n");
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
  }
numworkers = numtasks-1;


/**************************** master task ************************************/
   if (taskid == MASTER)
   {
      printf("MPI Neural Network has started with %d tasks.\n",numtasks);
      printf("Initializing arrays...\n");
      for (i=0; i<size; i++)
         {for (j=0; j<size; j++)
			{f=rand()%1000+1;
			f=f/1000;
            weight[i][j]= f;}
		 }
      for (i=0; i<size; i++)
		{
		for(j=0;j<size;j++)
			{
			if(j==0){
			f=rand()%1000+1;
			f=f/1000;
			layer[i][j]=f;
			}
			else{
			layer[i][j]=layer[i][j-1];
				}
			}
		}
     

      averow = size/numworkers;
      extra = size%numworkers;
      offset = 0;
      mtype = FROM_MASTER;
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&weight[offset][0], rows*size, MPI_FLOAT, dest, mtype,
                   MPI_COMM_WORLD);
         MPI_Send(&layer, size*size, MPI_FLOAT, dest, mtype, MPI_COMM_WORLD);
         offset = offset + rows;
      }



      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&output[offset][0], rows*size, MPI_FLOAT, source, mtype, 
                  MPI_COMM_WORLD, &status);
         printf("Received results from task %d\n",source);
      }

   }


/**************************** worker task ************************************/
   if (taskid > MASTER)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&weight, rows*size, MPI_FLOAT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&layer, size*size, MPI_FLOAT, MASTER, mtype, MPI_COMM_WORLD, &status);

      for (k=0; k<size; k++)
         for (i=0; i<rows; i++)
         {
            output[i][k] = 0.0;
            for (j=0; j<size; j++)
               {output[i][k] = output[i][k] + weight[i][j] * layer[j][k];}
			output[i][k]=sigmoid(output[i][k]);
         }
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&output, rows*size, MPI_FLOAT, MASTER, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}
