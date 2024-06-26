#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the values for each process
    int values[] = {5, 2, 9, 1, 13}; 

    // Determine left and right neighbours in the ring
    int left_neigh = (rank - 1 + size) % size;
    int right_neig = (rank + 1) % size;

    // Perform communication and computation for processes with rank greater than 0
    int left_num, right_num;
    if (rank != 0) {
        // Send and receive values to/from neighbours
        MPI_Send(&values[rank], 1, MPI_INT, right_neig, 0, MPI_COMM_WORLD);
        MPI_Recv(&left_num, 1, MPI_INT, left_neigh, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Perform comparison only for processes from 1 onwards
        if (values[rank] < left_num || values[rank] < values[left_neigh]) {
            printf("Process %d has number %d out of order.\n", rank, values[rank]);
        } 
    }
    else {
        MPI_Send(&values[rank], 1, MPI_INT, right_neig, 0, MPI_COMM_WORLD);
        MPI_Recv(&left_num, 1, MPI_INT, left_neigh, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    }

    int ooo_cnt = 0;

    if (rank != 0) {
        int check_ooo = (values[rank] < values[left_neigh]);
        MPI_Send(&check_ooo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            int check_ooo;
            MPI_Recv(&check_ooo, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (check_ooo) {
                ooo_cnt++;
            }
        }
    }

    if (rank == 0) {
        printf("The number of processes holding an out-of-order number is %d.\n", ooo_cnt);
    }

    MPI_Finalize();
    return 0;
}
