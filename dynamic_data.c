#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv){

	int world_rank;
	int world_size; 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	const int MAX_NUMBERS = 100;
	int numbers[MAX_NUMBERS];

	int number_amount;

	if(world_rank == 0){
		srand(time(NULL));
		number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
		
		MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
		printf("%d send %d numbers to 1\n", world_rank, number_amount);		

	} else if (world_rank == 1) {
		MPI_Status status;

		MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		MPI_Get_count(&status, MPI_INT, &number_amount);

		printf("%d received %d numbers from 0.  Message source = %d, tag = %d\n", world_rank, number_amount, status.MPI_SOURCE, status.MPI_TAG);
	}

	MPI_Finalize();

}
