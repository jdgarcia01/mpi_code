/**********************************************************
 ** MPI example program for numerical integration.       **
 ** Solve the equation: f(x) = cos^2(x) where x = 0 to   **
 ** M_PI.                                                **
 ** If you execute this program you should get 1.5708    **
 *********************************************************/ 

#include <stdio.h>
#include <math.h>
#include <mpi.h>

double func(double x){


	return pow(cos(x),2); 
}


double Trap(double left_endpt, double right_endpt, int trap_count, double base_len)
{

	double estimate, x;
	int i;

	estimate =(func(left_endpt) + func(right_endpt))/2.0;
	for( i = 1; i <= trap_count - 1; i++){
		x = left_endpt + i * base_len;
		estimate += func(x);
	}
	estimate = estimate * base_len;

	return estimate;
}

int main(void){

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int process_len; 

	int my_rank, comm_sz, n = 100000, local_n;
	double a = 0.0, b = M_PI, h, local_a, local_b;
	//double a = 10, b = 35, h, local_a, local_b;
	double local_int, total_int;
	int source;

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	MPI_Get_processor_name(processor_name, &process_len);

	// Partition out the work for each n Nodes.
	h = (b-a)/n;
	local_n = n/comm_sz;

	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;

	local_int = Trap(local_a, local_b, local_n, h);
	printf("Partition is: %lf to %lf\n", local_a, local_b); 

	// aggregate the results from each node.	
	MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM,0 ,MPI_COMM_WORLD);

	if(my_rank == 0){
		printf("With n = %d trapezoids, our estimate\n", n);
		//printf("of the intergral from %f to %f = %.15e\n", a,b,total_int);
		printf("of the intergral from %f to %f = %lf\n", a,b,total_int);

	}

	MPI_Finalize();
	return 0;



}
