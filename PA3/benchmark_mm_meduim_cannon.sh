#!/bin/bash
#SBATCH -J KERNELS # job name
#SBATCH -o ./benchmarks/mm/medium.o # output and error file name %j expands to jobID)
#SBATCH -n 1 # total number of tasks requested
#SBATCH -N 1 # number of nodes you want to run on
#SBATCH -p defq # queue (partition) -- defq, eduq, gpuq, short
#SBATCH -t 12:00:00 # run time (hh:mm:ss) - 2.0 mins in this example.
# Generally needed modules:
module load gcc8/8.4.0
module load slurm
module load anaconda/anaconda3/5.1.0
module load mpich/ge/gcc/64/3.2.1

make matrix-matrix-cannon

sizes=(1680)
#threads=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28)
#threads=(1 2 4 8 10 12 16 20 24)
#threads=(1)
threads=(1 4 9 16 25)

for i in "${sizes[@]}"
do
    echo "----------------------------------------------------"
    echo "Size of Output Matrix: $i x $i"
    echo "----------------------------------------------------"
    #python3 generate_matrix.py -m1 ./benchmarks/mm/medium_matrix43.mm -m2 ./benchmarks/mm/medium_vector43.mm -o ./benchmarks/mm/medium_output43.mm -s $i
    for j in "${threads[@]}"
    do
        echo "Number of mpi processors: $j"
        mpirun -np $j ./build/bin/matrix-matrix-cannon ./benchmarks/mm/medium_matrix43.mm ./benchmarks/mm/medium_vector43.mm
    done
done