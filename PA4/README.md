
## README for PA4

For PA4, we wrote parallelized version of mandelbrot using cuda and openmp.
We also created a build system and test suite that we should be able to use and/or build on in 
future projects.

Outcome of the Project:

![mandelbrot](https://user-images.githubusercontent.com/13769514/205765118-c781d854-0f21-401e-b531-4b8a32700be9.png)


## Contents of this README:

I.   Getting The Source Code

II.  Quick Start
         A. Building PA4
         
         B. Running Kernels for PA4
         
III.  Contact

## I. Getting The Source Code

The PA2 library is being actively developed, and new functionalities
are being added. You can get the latest source code from this git repository:

    git clone https://github.com/kalyanbhetwal/CS430
    


## II. Quick Start 
  A. Building PA4
  
  B. Running Kernels for PA4
  
# A. Building PA4

If you want to do a quick build, first check that you have required packages
installed (see Requirements section below). Also please load appropriate modules with commands:
         
    module load slurm
    module load cuda10.0
    
    CUDA:
     
    nvcc -O3 -o mandelbrot mandelbrot.cu png_util.c -I. -lm -lpng
    
    OPENMP:
    --------------------
    Use the following command to compile
    
    gcc -fopenmp mandelbrot-openmp.c -o mandelbrot-cuda
    


# B. Running Kernels for PA4
     
     CUDA:
     sbatch cuda_job.bash

     OPENMP:
     --------------------
     Before executing the program, in your bash/shell script or terminal run:

     export OMP_NUM_THREADS= <number of threads to use>

## III. Contact 

For more information please contact:

Kalyan Bhetwal, kalyanbhetwal@u.boisestate.edu

Phillip Bruce, phillipbruce@u.boisestate.edu

Paul Vanderveen, paulvanderveen@u.boisestate.edu

