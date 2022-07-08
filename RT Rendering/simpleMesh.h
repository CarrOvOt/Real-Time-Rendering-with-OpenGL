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
	SimpleMesh(SHAPE shape=CUBE);

	//void Draw(Shader& shader);

private:


};


