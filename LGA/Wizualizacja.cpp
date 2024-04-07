#include "Wizualizacja.h"
#include <iostream>
#include "kernel.cuh"

// Kody shaderów
const GLchar* vertexSource = R"glsl(
#version 150 core
in vec2 position;
in vec3 color;
out vec3 Color;


void main(){

Color = color;
gl_Position = vec4(position, 0.0, 1.0);
gl_PointSize = 10.0;
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;
out vec4 outColor;

void main()
{

outColor =  vec4(Color, 1.0);
}
)glsl";


void aplication(Cell* matrix, int rows, int cols)
{
	sf::ContextSettings settings;//tworzy objekt do konfiguracji ustawieñ kontekstu renderingu
	settings.depthBits = 24;//Ustawienie g³êbokoœci jak daleko
	settings.stencilBits = 8;//Ustawienie bufora szablonu

					// Okno renderingu
	sf::Window window(sf::VideoMode(cols, rows, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

					// Inicjalizacja GLEW, uaktywnia OpenGL
	glewExperimental = GL_TRUE;
	glewInit();
	
	

					// Utworzenie i skompilowanie shadera wierzcho³ków
	GLuint vertexShader =glCreateShader(GL_VERTEX_SHADER);// tworzy obiekt shadera wierzcho³ków
	glShaderSource(vertexShader, 1, &vertexSource, NULL);//Ustawia Ÿród³owy kod shadera wierzcho³ków
	glCompileShader(vertexShader);//Kompiluje shader wierzcho³ków

	GLint compilationSucces;
	GLchar log[512];



					// Sprawdza, czy kompilacja shadera wierzcho³ków zakoñczy³a siê sukcesem
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationSucces);
	if (!compilationSucces)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		std::cerr << "Compilation vertexShader Error\n" << log << std::endl;
	}
	else
	{
		std::cout << "Compilation vertexShader OK\n";
	}



					// Utworzenie i skompilowanie shadera fragmentów to samo co wyzej tylko dla fragmentow
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationSucces);
	if (!compilationSucces)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		std::cerr << "Compilation vertexShader Error\n" << log << std::endl;
	}
	else
	{
		std::cout << "Compilation vertexShader OK\n";


	}

					// Zlinkowanie obu shaderów w jeden wspólny program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);// Do³¹cza shader wierzcho³ków do programu cieniuj¹cego
	glAttachShader(shaderProgram, fragmentShader);//Do³¹cza shader fragmentów do programu cieniuj¹cego 
	glLinkProgram(shaderProgram);//£¹czy wszystkie do³¹czone shadery w jeden program
	//glUseProgram(shaderProgram);//Ustawia ten program jako aktualny program u¿ywany podczas procesu renderingu.

					// Utworzenie VBO (Vertex Buffer Object) i skopiowanie do niego danych wierzcho³kowych

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float* vertices = visualize(matrix, rows, cols);
	int points_number = rows * cols;
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points_number * 5, vertices, GL_STATIC_DRAW);
	delete[] vertices;

	// Utworzenie VAO (Vertex Array Object)
	GLuint vao;
	glGenVertexArrays(1, &vao);//inicjalizacja bufora
	glBindVertexArray(vao);//mozemy wprowadzac zmiany w buforze za pomoca id


					// Specifikacja formatu danych wierzcho³kowych, przekazujemy do bufora VAO w jaki sosob wierzcholki sa zapisane w tablicach
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position"); //Pobiera lokalizacjê atrybutu "position" z shaderProgram
	glEnableVertexAttribArray(posAttrib);//aktywuje atrybut wejsciowy position 
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);//Okreœla, jak OpenGL ma interpretowaæ dane dla atrybutu "position"
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glUseProgram(shaderProgram);

	Cell* matrixDevice;
	cudaMalloc(&matrixDevice, rows * cols * sizeof(Cell));//alokuje miejsce w karcie
	cudaMemcpy((void*) matrixDevice, matrix, rows * cols * sizeof(Cell), cudaMemcpyHostToDevice);//kopiuje z ramu do pamieci karty

	cudaGraphicsResource_t cudaResource = nullptr;//zmienna do ktorej zapisywane info o zasobach z ktorych mpozemy korzystac

	cudaError_t e = cudaGraphicsGLRegisterBuffer(&cudaResource, vbo, cudaGraphicsMapFlagsWriteDiscard);//pozwala bibliotece cuda korzystac z zasobu openGL ktorym jest vbo
	if (e != cudaSuccess)
	{
		std::cout << "Resources has not been registered!" << std::endl;
	}

	bool running = true;
	sf::Event windowEvent;

	while (running) 
	{
		window.pollEvent(windowEvent);
		switch (windowEvent.type)
		{
		case sf::Event::Closed:
			running = false;
			break;
		default:
			break;
		}
		//std::cout << "---ITERATION START---" << std::endl;

		run_kernels(matrixDevice, rows, cols);

		cudaGraphicsMapResources(1, &cudaResource, 0);
		float* vboDevices;
		size_t vboSize;
		cudaGraphicsResourceGetMappedPointer((void**)&vboDevices, &vboSize, cudaResource);
		draw(matrixDevice, rows, cols, vboDevices);
		cudaGraphicsUnmapResources(1, &cudaResource);

		//std::cout << "---ITERATION END---" << std::endl;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_POINTS, 0, rows * cols);


		window.display();
	}
	window.close();
	cudaFree(matrixDevice);
}