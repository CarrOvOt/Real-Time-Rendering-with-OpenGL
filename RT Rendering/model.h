#pragma once

#include "mesh.h"
#include "simpleMesh.h"

using namespace std;

class Model {
public:

	Model();
	void Draw(Shader& shader);

private:
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
};