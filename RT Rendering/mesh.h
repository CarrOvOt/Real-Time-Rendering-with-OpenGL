#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

// stb_image for texture loading
#include <stb_image.h>

#include <vector>
#include <string>
#include <iostream>

#include "shader.h"
#include "camera.h"

using namespace std;

enum TEXTURE_TYPE {
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT
};

struct Vertex {
	// position
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	TEXTURE_TYPE type;
	string name;
};


class Mesh {

public:

	glm::mat4 Transform = glm::mat4(1.0f);

	Mesh();
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);


	void Draw(Shader& shader, Camera& camera, glm::mat4 parent_trans);
	void Draw(Shader& shader); // draw without setting any uniform vars, for screen texture drawing.


protected:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> Vertices;
	vector<unsigned int> Indices;
	vector<Texture> Textures;

	void setupMesh();


};
