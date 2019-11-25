#include "node.hpp"
#include "mpi_types.hpp"

#include <iostream>
#include <mpi.h>

void Create_Nodes(int& argc, char* argv[]);

// Compiled using: mpic++ -o <outputfile> ./main.cpp
// Run: mpirun -n <number of processors> ./<outputfile>

int main(int argc, char* argv[]) {
    // Create the Nodes && start the work
    MPI_Init(&argc, &argv);
    init_mpi_types();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Node created_node(rank);
    created_node.start_node_work();

    free_mpi_types();
    MPI_Finalize();
}
