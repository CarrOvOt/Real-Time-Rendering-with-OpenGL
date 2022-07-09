#pragma once

#include <glad/glad.h>

// stb_image for texture loading
#include <stb_image.h>

#include <vector>
#include <iostream>

#include "mesh.h"

using namespace std;

enum SHAPE {
	CUBE
};



class SimpleMesh: public Mesh{

public:
	SimpleMesh();
	SimpleMesh(SHAPE shape, glm::vec3 color);

	void Draw(Camera& camera, glm::mat4 parent_trans) override;

private:
	glm::vec3 Color = glm::vec3(0.5f, 0.5f, 0.5f);
		
};


