#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
extern "C" {
  #include "png_util.h"
}

__global__ void mandelbrotKernel(const int NRe, 
		const int NIm, 
		const float xmin,
		const float ymin,
		const float dRe, 
		const float dIm,
		float * h_count){

  // orient thread
  int tx = threadIdx.x;
  int bx = blockIdx.x;
  int dx = blockDim.x;
  
  int ty = threadIdx.y;
  int by = blockIdx.y;
  int dy = blockDim.y;
  
  // establish which entry this thread is responsible for
  int column = tx + bx*dx;
  int row    = ty + by*dy;

  if (row < NIm && column < NRe) { // bounds check
      float cRe = xmin + column*dRe;
      float cIm = ymin + row*dIm;

      float zRe = 0;
      float zIm = 0;
      
      int Nt = 200;
      int t, cnt=0;
      for(t=0;t<Nt;++t){
	
	// z = z^2 + c
	//   = (zRe + i*zIm)*(zRe + i*zIm) + (cRe + i*cIm)
	//   = zRe^2 - zIm^2 + 2*i*zIm*zRe + cRe + i*cIm
	      float zReTmp = zRe*zRe - zIm*zIm + cRe;
	      zIm = 2.f*zIm*zRe + cIm;
	      zRe = zReTmp;

	      cnt += (zRe*zRe+zIm*zIm<4.f);
      }

      h_count[column + row*NRe] = cnt;
  }

}


int main(int argc, char **argv){

  const int NRe = 1000;
  const int NIm = 1000;

  const float xmin = -2;
  const float xmax = 1; 
  const float ymin = -1.5; 
  const float ymax = 1.5;

  const float dRe = (xmax-xmin)/(NRe-1.f);
  const float dIm = (ymax-ymin)/(NIm-1.f);

  // allocate HOST array
  float *h_count = (float*) calloc(NRe*NIm, sizeof(float));

  // allocate DEVICE array
  float* c_count;
  cudaMalloc(&c_count, NRe*NIm * sizeof(float));

  // establish threading dimensions
  dim3 B(16,16,1);
  dim3 G((NIm+16-1)/16, (NRe+16-1)/16, 1);

  // warm up kernel
  mandelbrotKernel <<< G , B >>> (NRe, NIm, xmin, ymin, dRe, dIm, c_count);

  // set up timing
  cudaEvent_t tic, toc;
  cudaEventCreate(&tic);
  cudaEventCreate(&toc);

  // start timer
  cudaDeviceSynchronize();
  cudaEventRecord(tic);

  // call mandelbrot from here
  mandelbrotKernel <<< G , B >>> (NRe, NIm, xmin, ymin, dRe, dIm, c_count);

  // end timer
  cudaEventRecord(toc);
  cudaDeviceSynchronize();

  // get and print elapsed time
  float elapsed;
  cudaEventElapsedTime(&elapsed, tic, toc);
  elapsed /= 1000;
  
  printf("Elapsed Time %f seconds\n", elapsed);

  // copy results from DEVICE to HOST
  cudaMemcpy(h_count, c_count, NRe*NIm * sizeof(float), cudaMemcpyDeviceToHost);

  // create the png
  FILE *png = fopen("mandelbrot.png", "w");
  write_hot_png(png, NRe, NIm, h_count, 0, 100);
  fclose(png);

}