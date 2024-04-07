#pragma once
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include "Symulacja.h"
#include <cuda_gl_interop.h>



//struct OpenGLDate
//{
//	GLuint VBO; //id bufora w którym beda dane o wierzcho³kach(polozenie +kolor)
//	GLuint VAO; //id bufora w ktorym bedzie info jak odczytywac wierzcholki
//
//};


void aplication(Cell* matrix, int rows, int cols);
