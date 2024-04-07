#include "Symulacja.h"
#include <iostream>
#include <stdlib.h>







Cell* createMatrix(int rows, int cols)
{
	Cell* cell = new Cell[rows*cols];

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (j == 0 || i == 0 || i == rows - 1 || j == cols - 1)
			{
				cell[i * cols + j].wall = 1;
			}
			else cell[i * cols + j].wall = 0;

			for (int k = 0; k < 4; k++)
			{
				cell[i*cols + j].inTab[k] = 0;
				cell[i*cols + j].outTab[k] = 0;
			}
			cell[i * cols + j].C = 0;
		}


	}

	return cell;

}

void wall(Cell* matrix, int rows, int cols)
{

	int colWall = cols / 3;
	int rowWallGap = rows / 3;

	for (int i = 0; i < rows; i++)
	{
		if (i > rowWallGap && i < rowWallGap * 2) continue;
		else
		{
			matrix[i * cols + colWall].wall = 1;
		}
	}

}


void rand(Cell* matrix, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols/3 -1; j++)
		{
			
				//if ((rand() % 100) > 80 )
				matrix[i * cols + j].C = 1;
				// matrix[i * cols + j].C = 0;
			
			
		}
	}
}
//do edytowania

/*void algo(Cell* matrix, int rows, int cols)
{
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if(matrix[i*cols + j].wall == 1 )
			{
				matrix[i*cols + j].outTab[0] = matrix[i*cols + j].inTab[0];
				matrix[i*cols + j].outTab[1] = matrix[i*cols + j].inTab[1];
				matrix[i*cols + j].outTab[2] = matrix[i*cols + j].inTab[2];
				matrix[i*cols + j].outTab[3] = matrix[i*cols + j].inTab[3];

			}
			if (matrix[i*cols + j].inTab[0] == matrix[i*cols + j].inTab[1] && matrix[i*cols + j].inTab[2] != matrix[i*cols + j].inTab[3])
			{ 
				matrix[i*cols + j].outTab[0] = matrix[i*cols + j].inTab[0];
				matrix[i*cols + j].outTab[1] = matrix[i*cols + j].inTab[0];
				matrix[i*cols + j].outTab[2] = matrix[i*cols + j].inTab[3];
				matrix[i*cols + j].outTab[3] = matrix[i*cols + j].inTab[2];

			}
			if (matrix[i*cols + j].inTab[0] != matrix[i*cols + j].inTab[1] && matrix[i*cols + j].inTab[2] == matrix[i*cols + j].inTab[3])
			{
				matrix[i*cols + j].outTab[0] = matrix[i*cols + j].inTab[1];
				matrix[i*cols + j].outTab[1] = matrix[i*cols + j].inTab[0];
				matrix[i*cols + j].outTab[2] = matrix[i*cols + j].inTab[2];
				matrix[i*cols + j].outTab[3] = matrix[i*cols + j].inTab[2];
			}
			if (matrix[i*cols + j].inTab[0] == matrix[i*cols + j].inTab[1] && matrix[i*cols + j].inTab[2] == matrix[i*cols + j].inTab[3])
			{
				matrix[i*cols + j].outTab[0] = matrix[i*cols + j].inTab[2];
				matrix[i*cols + j].outTab[1] = matrix[i*cols + j].inTab[2];
				matrix[i*cols + j].outTab[2] = matrix[i*cols + j].inTab[0];
				matrix[i*cols + j].outTab[3] = matrix[i*cols + j].inTab[0];
			}
			if (matrix[i*cols + j].inTab[0] != matrix[i*cols + j].inTab[1] && matrix[i*cols + j].inTab[2] != matrix[i*cols + j].inTab[3])
			{
				matrix[i*cols + j].outTab[0] = matrix[i*cols + j].inTab[1];
				matrix[i*cols + j].outTab[1] = matrix[i*cols + j].inTab[0];
				matrix[i*cols + j].outTab[2] = matrix[i*cols + j].inTab[3];
				matrix[i*cols + j].outTab[3] = matrix[i*cols + j].inTab[2];
			}


			
		}
	}

}*/
//do edytowania

void outToIn(Cell* matrix, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			matrix[(i-1) * cols + j].inTab[0] = matrix[i*cols + j].outTab[1];
			matrix[(i+1) * cols + j].inTab[1] = matrix[i*cols + j].outTab[0];
			matrix[i * cols + (j-1)].inTab[2] = matrix[i*cols + j].outTab[3];
			matrix[i * cols + (j+1)].inTab[3] = matrix[i*cols + j].outTab[2];

			
		}
	}
}



float* visualize(Cell* matrix, int rows, int cols)
{
	float* visualizeTab = new float[rows*cols*5];
	int norm;

	int suma = 0;

	if (rows < cols)norm = cols;
	else norm = rows;


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			
			visualizeTab[(i * cols + j) * 5] = 2.0 * ((float) j )/((float) norm) - 1.0;
			visualizeTab[((i * cols + j) * 5)+1] = 2.0 * ((float)i) / ((float)norm) - 1.0;
			if (matrix[i * cols + j].wall)
			{
				visualizeTab[((i * cols + j) * 5) + 2] = 1.0;
				visualizeTab[((i * cols + j) * 5) + 3] = 0;
				visualizeTab[((i * cols + j) * 5) + 4] = 0;
				continue;
			}
			suma = matrix[i * cols + j].outTab[0] + matrix[i * cols + j].outTab[1] + matrix[i * cols + j].outTab[2] + matrix[i * cols + j].outTab[3];
			visualizeTab[((i * cols + j) * 5) + 2] = 0;
			visualizeTab[((i * cols + j) * 5) + 3] = 0;
			visualizeTab[((i * cols + j) * 5) + 4] = matrix[i * cols + j].C;
			/*if (suma = 4)
			{
				visualizeTab[((i * cols + j) * 5) + 2] = 0;
				visualizeTab[((i * cols + j) * 5) + 3] = 1;
				visualizeTab[((i * cols + j) * 5) + 4] = 0;
				continue;
			}
			if (suma = 3)
			{
				visualizeTab[((i * cols + j) * 5) + 2] = 0;
				visualizeTab[((i * cols + j) * 5) + 3] = 0;
				visualizeTab[((i * cols + j) * 5) + 4] = 1;
				continue;
			}
			if (suma = 2)
			{
				visualizeTab[((i * cols + j) * 5) + 2] = 1;
				visualizeTab[((i * cols + j) * 5) + 3] = 0;
				visualizeTab[((i * cols + j) * 5) + 4] = 0;
				continue;
			}
			if (suma = 1)
			{
				visualizeTab[((i * cols + j) * 5) + 2] = 1;
				visualizeTab[((i * cols + j) * 5) + 3] = 0;
				visualizeTab[((i * cols + j) * 5) + 4] = 0.5;
				continue;
			}
			if (suma = 0)
			{
				visualizeTab[((i * cols + j) * 5) + 2] = 0;
				visualizeTab[((i * cols + j) * 5) + 3] = 0.5;
				visualizeTab[((i * cols + j) * 5) + 4] = 1;
				continue;
			}*/
		}
	}
	return visualizeTab;
}


