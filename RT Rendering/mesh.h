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

	glm::mat4 Tranform = glm::mat4(1.0f);

	Mesh();
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void virtual Draw(Camera& camera, glm::mat4 parent_trans); // Draw with default shader, and now ONLY SimpleMesh use this.

	void Draw(Shader& shader, Camera& camera, glm::mat4 parent_trans);


protected:
	unsigned int VAO, VBO, EBO;
	vector<Vertex> Vertices;
	vector<unsigned int> Indices;
	vector<Texture> Textures;

	Shader DefaultShader;

	void setupMesh();


};
