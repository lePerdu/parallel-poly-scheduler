#include <iostream>
#include <mpi.h>
#include "node.cpp"

void Create_Nodes(int& argc, char* argv[]);

// Compiled using: mpic++ -o <outputfile> ./main.cpp
// Run: mpirun -n <number of processors> ./<outputfile>

int main(int argc, char* argv[]) {
  //  Create the Nodes && start the work
  MPI::Init(argc, argv);

  int rank = rank = MPI::COMM_WORLD.Get_rank();
  Node created_node(rank);
  created_node.start_node_work();

  MPI_Barrier(MPI::COMM_WORLD);
  MPI::Finalize();
}
