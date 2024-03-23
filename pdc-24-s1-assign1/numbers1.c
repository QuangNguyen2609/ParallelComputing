#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size, my_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the numbers for each process
    // read input array from user
    srand(rank);
    my_num = rand() % 100;

    // Determine left and right neighbours in the ring
    int left_neighbour = (rank - 1 + size) % size;
    int right_neighbour = (rank + 1) % size;

    // Send and receive numbers to/from neighbours
    int left_number, right_number;
    MPI_Send(&my_num, 1, MPI_INT, right_neighbour, 0, MPI_COMM_WORLD);
    MPI_Recv(&left_number, 1, MPI_INT, left_neighbour, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Determine if the number is out of order
    // print number rank

    if (my_num < left_number) {
        printf("Process %d has number %d out of order.\n", rank, my_num);
        printf("Process %d has finished.\n", rank);
    } 

    // Print termination message

    MPI_Finalize();
    return 0;
}


