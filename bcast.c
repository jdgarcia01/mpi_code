#include <stdio.h>
#include <mpi.h>


int main(int argc, char **argv){

	MPI_Init(&argc, &argv);

	int rank, size;
	double total_mpi_bcast_time = 0.0;

	MPI_Comm_size( MPI_COMM_WORLD, &size);
	MPI_Comm_rank( MPI_COMM_WORLD, &rank);

	int data = 100;
	int num_elements = 1;

	MPI_Barrier(MPI_COMM_WORLD);
	total_mpi_bcast_time -= MPI_Wtime();	

	MPI_Bcast(&data
		 ,num_elements
		 ,MPI_INT
		 ,0
		 ,MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	total_mpi_bcast_time += MPI_Wtime();
	if( rank == 0){
		printf("MPI_Bcast time %lf\n", total_mpi_bcast_time);
	}
	if( rank != 0){
		printf("process %d received %d\n", rank, data * rank);
	}

}
