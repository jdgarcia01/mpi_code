#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define MIN(a,b)           ((a)<(b)?(a):(b))
#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p)) // rank * number of elements / num of processors.  
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) \
                     (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(j,p,n) (((p)*((j)+1)-1)/(n))
#define PTR_SIZE           (sizeof(void*))
#define CEILING(i,j)       (((i)+(j)-1)/(j))

/**********************************************************
 ** Find all primes bwtween n and m ending point         **
 ** Primality test using 6k+-1.                          **
 *********************************************************/
unsigned long long prime_test(unsigned long long test_num){

	if( test_num % 2 == 0 || test_num % 3 == 0 ){
		return 0;	

	} 
	int i = 5;
	while( pow(i,2) <= test_num){
		if( test_num % i == 0 || test_num % (i + 2) == 0){
			return 0;
		}
		i += 6;
	}
	return test_num;
}



int main(int argc, char **argv){

	MPI_Init(&argc, &argv);
		
	
	int rank, size,  local_n;
        unsigned long long int a, b, h, local_a, local_b;
	unsigned long long int number_of_elements;
	unsigned long long int total_number_array[number_of_elements + 1]; 
	

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	a = atol(argv[1]);
	b = atol(argv[2]);
	//a = 2000;
	//b = 4000;	
		
        // Partition out the work for each n Nodes.
	// number of elements between a and b.
	number_of_elements = b - a;

	// Determine the chunk/slice of each array that an individual CPU will 
	// work on.  local_n is the chunk/slice size for each CPU.
	unsigned long long int block_size = BLOCK_SIZE(rank, size, number_of_elements);

	local_n = block_size; // local chunk/slice for each local CPU.

	// Determine the local starting for each CPU.	
	local_a = BLOCK_LOW(rank, size, number_of_elements) + a;

	// Determine the local ending point for each CPU. 
	local_b = BLOCK_HIGH(rank, size, number_of_elements) + a;

        printf("block_size %d\n",block_size);
	printf("proc %d local_ is %lld\n",rank, local_n);
	printf("proc %d local_a is %lld\n",rank, local_a);
	printf("proc %d local_b is %lld\n",rank, local_b);
	
	unsigned long long int numbers[local_n]; 

	unsigned long long int number; 
	unsigned long long int answer; 

	if(rank == 0){
		// Process our portion of the work.
		printf("procs %d local_a is: %lld\n", rank, local_a);
		printf("procs %d local_b is: %lld\n", rank, local_b);

		for(int i = local_a; i < local_b; i++){
			number = prime_test(i);
			if(number > 0){
				printf("possible prime: %lld\n", number);

			}
		} 

      	 for(int i = 1; i < size; i++){
		unsigned long long int buf;
	

		MPI_Recv(&answer, 10, MPI_UNSIGNED_LONG_LONG, i ,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
		printf("possible prime: %lld \n", answer);	
		 }	
			
	 		
	}

	unsigned long long prime_numbers;
	if( rank != 0) {
		for(int i = local_a; i < local_b; i++){
			number = prime_test2(i);
			if(number > 0 ){			
				MPI_Send(&number, 10, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
			}

		}
	} 
	MPI_Finalize();	
	return 0; 

}
