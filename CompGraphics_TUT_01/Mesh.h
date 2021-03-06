#pragma once
#include <string>
#include <GL\glew.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Load a mesh and send it to OpenGL
	bool loadFromFile(const std::string &file);

	// Release data from OpenGL (VRAM)
	void unload();


	unsigned int getNumFaces();
	unsigned int getNumVertices();

	//OpengGL buffers and objects
	GLuint vboVertices = 0; // vbo (vertex buffer object)
	GLuint vboUVs = 0;
	GLuint vboNormals = 0;
	GLuint vao = 0; // vertex array object

	unsigned int numFaces = 0;
	unsigned int numVertices = 0;
};
