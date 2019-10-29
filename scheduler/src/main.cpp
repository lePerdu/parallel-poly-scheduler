#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI::Init(argc, argv);

    const auto node_count = MPI::COMM_WORLD.Get_size();
    const auto rank = MPI::COMM_WORLD.Get_rank();

    std::cout << "Node " << rank << " of " << node_count << std::endl;

    MPI::Finalize();
}
