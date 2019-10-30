# parallel-poly-scheduler

Parallel scheduler implementation using MPI.

## Building

On UNIX-like systems (i.e. Linux, macOS, or WSL):

``` shell
# This is only needed the first time
git submodule init
git submodule update

# Actually build the program
make
```

## Running

After building, the executable will be at `bin/parallel_scheduler`. This can
then be run using `mpirun`.
