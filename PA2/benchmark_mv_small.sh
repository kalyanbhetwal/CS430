#!/bin/bash
#SBATCH -J KERNELS # job name
#SBATCH -o ./benchmarks/mv/small.o # output and error file name %j expands to jobID)
#SBATCH -n 1 # total number of tasks requested
#SBATCH -N 1 # number of nodes you want to run on
#SBATCH -p defq # queue (partition) -- defq, eduq, gpuq, short
#SBATCH -t 12:00:00 # run time (hh:mm:ss) - 2.0 mins in this example.
# Generally needed modules:
module load gcc8/8.4.0
module load slurm
module load anaconda/anaconda3/5.1.0

make matrix-vector_main 

sizes=(480)
threads=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28)

export OMP_NUM_THREADS=1
for i in "${sizes[@]}"
do
    echo "----------------------------------------------------"
    echo "Size of Output Vector: $i x 1"
    echo "----------------------------------------------------"
    python3 generate_matrix.py -m1 ./benchmarks/mv/small_matrix43.mm -m2 ./benchmarks/mv/small_vector43.mm -o ./benchmarks/mv/small_output43.mm -s $i -v
    for j in "${threads[@]}"
    do
        export OMP_NUM_THREADS=$j
        echo "Number of OMP Threads: $j"
        ./build/bin/matrix-vector_main ./benchmarks/mv/small_matrix43.mm ./benchmarks/mv/small_vector43.mm
    done
done
