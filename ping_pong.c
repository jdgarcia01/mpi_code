#include <mpi.h>
#include <stdio.h>
#include <string.h>

int multiply_number(int i, int rank){
	
	return i * rank; 
}

int main(int argc, char **argv){

	// Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int number;
  int answer;
  if (world_rank == 0) {
    number = 2;
    for(int i = 1; i < world_size; i++){
    MPI_Send(
      /* data         = */ &number,
      /* count        = */ 1,
      /* datatype     = */ MPI_INT,
      /* destination  = */ i,
      /* tag          = */ 0,
      /* communicator = */ MPI_COMM_WORLD);
    }
    for(int i = 1; i < world_size; i++){
	MPI_Recv( &answer, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	printf("Process %d, received the answer %d from proc: %d\n",world_rank, answer, i);
    }
  } else if (world_rank != 0) {

    MPI_Recv(
      /* data         = */ &number,
      /* count        = */ 1,
      /* datatype     = */ MPI_INT,
      /* source       = */ 0,
      /* tag          = */ 0,
      /* communicator = */ MPI_COMM_WORLD,
      /* status       = */ MPI_STATUS_IGNORE);
    printf("Process %d received number %d from process 0\n",world_rank, number);

    answer  = multiply_number(number, world_rank);

    MPI_Send(&answer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
   }
  MPI_Finalize();



}
