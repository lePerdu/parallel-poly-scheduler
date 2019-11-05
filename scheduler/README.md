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

To turn on more optimizations, use `make PRODUCTION=1` (`make clean` should
probably be done first to make sure all files are compiled with the
optimizations). Note that this also turns off assertions (runtime checks that
data makes sense), and so should only be used once the "non-production" build
has been tested and works as expected.


## Running

After building, the executable will be at `bin/parallel_scheduler`. This can
then be run using `mpirun`.
