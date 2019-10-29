# parallel-poly-scheduler

Parallel scheduler implementation using MPI.

## Building

On UNIX-like systems (i.e. Linux, macOS, or WSL):

``` shell
# This is only needed the first time
git submodule init
git submodule update

# Create a build directory
mkdir build
cd build

# Initialize CMake inside that directory
# This only has to be done if a new file is added or build files are changed
cmake ..

# Actually build the program
make
```

## Running

After building, the executable will be at `build/parallel_scheduler`. This can
then be run using `mpirun`.
