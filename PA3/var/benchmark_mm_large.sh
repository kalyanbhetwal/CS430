#!/bin/bash
#SBATCH -J KERNELS # job name
#SBATCH -o ./benchmarks/mm/large.o # output and error file name %j expands to jobID)
#SBATCH -n 1 # total number of tasks requested
#SBATCH -N 1 # number of nodes you want to run on
#SBATCH -p defq # queue (partition) -- defq, eduq, gpuq, short
#SBATCH -t 96:00:00 # run time (hh:mm:ss) - 2.0 mins in this example.
# Generally needed modules:
module load gcc8/8.4.0
module load slurm
module load anaconda/anaconda3/5.1.0

make matrix-matrix_main 

sizes=(4800)
threads=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28)
export OMP_NUM_THREADS=1
for i in "${sizes[@]}"
do
    echo "----------------------------------------------------"
    echo "Size of Output Vector: $i x 1"
    echo "----------------------------------------------------"
    #python3 generate_matrix.py -m1 ./benchmarks/mm/large_matrix43.mm -m2 ./benchmarks/mm/large_vector43.mm -o ./benchmarks/mm/large_output43.mm -s $i
    for j in "${threads[@]}"
    do
        export OMP_NUM_THREADS=$j
        echo "Number of OMP Threads: $j"
        ./build/bin/matrix-matrix_main ./benchmarks/mm/large_matrix43.mm ./benchmarks/mm/large_vector43.mm
    done
done
