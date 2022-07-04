#pragma once

#include <glad/glad.h>

#include <vector>

using namespace std;


struct Vertex {
	// position
	float Position[3];
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

	void setupMesh();


};


