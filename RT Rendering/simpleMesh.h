#pragma once

#include <glad/glad.h>

// stb_image for texture loading
#include <stb_image.h>

#include <vector>
#include <iostream>

using namespace std;


struct Vertex {
	// position
	float Position[3];
	float TexCoord[2];
};



enum SHAPE {
	CUBE
};

class SimpleMesh {

public:
	SimpleMesh(SHAPE shape);
	void Draw();


private:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int texture;

	void setupMesh();


};


