#include "Symulacja.h"
#include<iostream>
#include"Wizualizacja.h"

int main()
{
	cudaDeviceProp prop;
	int device;

	cudaGetDevice(&device);
	cudaGetDeviceProperties(&prop, device);

	std::cout << "Device " << device << ": " << prop.name << std::endl;
	std::cout << "Compute Capability: " << prop.major << "." << prop.minor << std::endl;
	int rows = 1024;
	int cols = 1024;
	Cell* matrix = createMatrix(rows, cols);
	wall(matrix, rows, cols);
	rand(matrix, rows, cols);
	aplication(matrix, rows, cols);
	delete[] matrix;
	return 0;
}