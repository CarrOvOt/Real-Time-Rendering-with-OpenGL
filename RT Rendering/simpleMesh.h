#pragma once

#include <glad/glad.h>

// stb_image for texture loading
#include <stb_image.h>

#include <vector>
#include <iostream>

#include "mesh.h"

using namespace std;

enum SHAPE {
	CUBE,
	RECT
};



class SimpleMesh: public Mesh{

public:

	SimpleMesh();
	SimpleMesh(SHAPE shape);

	void RemoveTextures();


private:

	Texture loadTexture(string file_path, TEXTURE_TYPE type);
	
};


