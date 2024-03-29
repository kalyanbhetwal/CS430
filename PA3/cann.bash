#!/bin/bash
#SBATCH -J MPI_TEST # job name
#SBATCH -o mpi_log.o # output and error file name (%j expands to jobID)
#SBATCH -n 28 # total number of tasks requested
#SBATCH -N 1 # number of nodes you want to run on
#SBATCH -p defq # queue (partition)
#SBATCH -t 12:00:00 # run time (hh:mm:ss) - 12.0 hours in this example.

module load slurm
module load mpich/ge/gcc/64/3.2.1
module load gcc8/8.4.0

make matrix-vector_main
mpirun -np 16 ./build/bin/matrix-vector_main  ./benchmarks/mv/small_matrix43.mm ./benchmarks/mv/small_vector43.mm

