#!/bin/bash
#SBATCH -J CUDA_TEST # job name
#SBATCH -o cuda.o% # output and error file name (%j expands to jobID)
#SBATCH -n 1 # total number of tasks requested
#SBATCH -N 1 # number of nodes you want to run on
#SBATCH -p gpuq # queue (partition) -- defq, eduq, gpuq, shortq
#SBATCH --gres=gpu:1
#SBATCH -t 12:00:00 # run time (hh:mm:ss) - 12.0 hours in this example.

module load slurm
module load cuda10.0

./mandelbrot 
