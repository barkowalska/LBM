
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "kernel.cuh"
#include<iostream>

__constant__ double t = 1;
__constant__ double tau = 2;


void run_kernels(Cell* matrix, int rows, int cols)//to wywolam z jakiegos cpp bo w cpp nie moge wywolac nic bezposrednio na karte graf cos jak getter w klasach
{
	dim3 blok = { 32,16,1 };//wymiar poj bloku ile watkow
	dim3 grid = { (cols + blok.x - 1) / blok.x, (rows + blok.y - 1) / blok.y,1 };//soatka watkow
	
	kernel_input << < grid, blok >> > (matrix, rows, cols);//uruchamiam kernela na siatce watkow ktorej wymiary sa opisane przez grid i przez blok
	cudaDeviceSynchronize();
	kernel_output << < grid, blok >> > (matrix, rows, cols);
	cudaDeviceSynchronize();
	
	 
}
__global__ void kernel_input(Cell* matrix, int rows, int cols)
{
	double w = 1.0 / 4.0;
	double feq[4] = { 0 };
	int index = threadIdx.x + blockDim.x * blockIdx.x + cols * (threadIdx.y + blockDim.y * blockIdx.y); //threadIdx- podaje wspolrzedne watku wew bloku, blockDim- wymiary bloku, blockIdx- podaje wspolrzedne bloku
	//index w ktorym el macierzy sie znjaduje
	if (matrix[index].wall == 1)
	{
		return;
	}
	for (int i = 0; i < 4; i++)
	{
		
		feq[i] = w * matrix[index].C;
		matrix[index].outTab[i] = matrix[index].inTab[i] + t / tau*(feq[i] - matrix[index].inTab[i]);
	}
}
__global__ void kernel_output(Cell* matrix, int rows, int cols)
{
	int index = threadIdx.x + blockDim.x * blockIdx.x + cols * (threadIdx.y + blockDim.y * blockIdx.y); //threadIdx- podaje wspolrzedne watku wew bloku, blockDim- wymiary bloku, blockIdx- podaje wspolrzedne bloku
	if (matrix[index].wall == 0)
	{
		if (matrix[index - cols].wall == 0)
			matrix[index].inTab[0] = matrix[index - cols].outTab[0];//w dol
		else
			matrix[index].inTab[0] = matrix[index].outTab[1];//w dol

		if (matrix[index + cols].wall ==0)
			matrix[index].inTab[1] = matrix[index + cols].outTab[1];
		else
			matrix[index].inTab[1] = matrix[index].outTab[0];//w dol

		if (matrix[index -1].wall == 0)
			matrix[index].inTab[2] = matrix[index - 1].outTab[2];//w prawo
		else
			matrix[index].inTab[2] = matrix[index].outTab[3];//w dol

		if (matrix[index + 1].wall == 0)
			matrix[index].inTab[3] = matrix[index + 1].outTab[3];
		else
			matrix[index].inTab[3] = matrix[index].outTab[2];//w dol


		matrix[index].C = matrix[index].inTab[0] + matrix[index].inTab[1] + matrix[index].inTab[2] + matrix[index].inTab[3];
	}
}

__global__ void kernel_draw(Cell* matrix, int rows, int cols, float* vbo)
{
	int index = threadIdx.x + blockDim.x * blockIdx.x + cols * (threadIdx.y + blockDim.y * blockIdx.y); //threadIdx- podaje wspolrzedne watku wew bloku, blockDim- wymiary bloku, blockIdx- podaje wspolrzedne bloku

	if (matrix[index].wall == 1)
	{
		vbo[((index) * 5) + 2] = 1.0f;
		vbo[((index) * 5) + 3] = 0;
		vbo[((index) * 5) + 4] = 0;
		return;
	}
	
	vbo[((index) * 5) + 2] =  matrix[index].C;
	vbo[((index) * 5) + 3] = matrix[index].C;
	vbo[((index) * 5) + 4] = matrix[index].C;
	
}


void draw(Cell* matrix, int rows, int cols, float* vbo)
{
	dim3 blok = { 32, 16, 1 };
	dim3 grid = { (cols + blok.x - 1) / blok.x, (rows + blok.y - 1) / blok.y,1 };
	kernel_draw <<< grid, blok >>> (matrix, rows, cols, vbo);
	cudaDeviceSynchronize();
}