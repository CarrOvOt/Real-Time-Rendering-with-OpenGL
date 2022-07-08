#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

// stb_image for texture loading
#include <stb_image.h>

#include <vector>
#include <string>
#include <iostream>

#include "shader.h"

using namespace std;

enum TEXTURE_TYPE {
	DIFFUSE
};

struct Vertex {
	// position
	glm::vec3 Position;
	glm::vec2 TexCoord;
};

struct Texture {
	unsigned int id;
	TEXTURE_TYPE type;
};


class Mesh {

public:
	Mesh();
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void Draw(Shader& shader);


protected:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	void setupMesh();


};
