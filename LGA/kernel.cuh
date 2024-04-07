#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Symulacja.h"

void run_kernels(Cell* matrix, int rows, int cols);
__global__ void kernel_input(Cell* matrix, int rows, int cols);
__global__ void kernel_output(Cell* matrix, int rows, int cols);

void draw(Cell* matrix, int rows, int cols, float* vbo);