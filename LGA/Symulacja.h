#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

struct Cell
{
	double outTab[4];
	double inTab[4];
	bool wall;
	double C;
};



Cell* createMatrix(int rows, int cols);
void wall(Cell* matrix, int rows, int cols);
void rand(Cell* matrix ,int rows, int cols);
void algo(Cell* matrix, int rows, int cols);
void outToIn(Cell* matrix, int rows, int cols);
float* visualize(Cell* matrix, int rows, int cols);
void symulacja(Cell* matrix, int rows, int cols);