#!/bin/bash
#SBATCH -J KERNELS # job name
#SBATCH -o ./benchmarks/mv/small.o # output and error file name %j expands to jobID)
#SBATCH -N 1 # number of nodes you want to run on
#SBATCH -p defq # queue (partition) -- defq, eduq, gpuq, short
#SBATCH -t 12:00:00 # run time (hh:mm:ss) - 2.0 mins in this example.
# Generally needed modules:

module load gcc8/8.4.0
module load slurm
module load anaconda/anaconda3/5.1.0
module load mpich/ge/gcc/64/3.2.1

make matrix-matrix_main 

mpirun -np 1 ./build/bin/matrix-matrix_main ./var/test.mm ./var/test2.mm