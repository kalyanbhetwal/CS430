#!/bin/bash
#SBATCH -J KERNELS # job name
#SBATCH -o ./benchmarks/mv/medium.o # output and error file name %j expands to jobID)
#SBATCH -n 1 # total number of tasks requested
#SBATCH -N 1 # number of nodes you want to run on
#SBATCH -p defq # queue (partition) -- defq, eduq, gpuq, short
#SBATCH -t 12:00:00 # run time (hh:mm:ss) - 2.0 mins in this example.
# Generally needed modules:

module load gcc8/8.4.0
module load slurm
module load anaconda/anaconda3/5.1.0
module load mpich/ge/gcc/64/3.2.1
make matrix-vector_main 

sizes=(3240)
threads=(1 2 4 6 8 10 12 16 20 24)

for i in "${sizes[@]}"
do
    echo "----------------------------------------------------"
    echo "Size of Output Vector: $i x 1"
    echo "----------------------------------------------------"
    python3 generate_matrix.py -m1 ./benchmarks/mv/small_matrix43.mm -m2 ./benchmarks/mv/small_vector43.mm -o ./benchmarks/mv/small_output43.mm -s $i -v
    for j in "${threads[@]}"
    do
        echo "Number of mpi processors: $j"
        mpirun -np $j ./build/bin/matrix-vector_main ./benchmarks/mv/small_matrix43.mm ./benchmarks/mv/small_vector43.mm
    done
done
