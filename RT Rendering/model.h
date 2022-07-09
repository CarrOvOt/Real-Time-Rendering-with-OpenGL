#pragma once

#include "mesh.h"
#include "simpleMesh.h"
#include "camera.h"

using namespace std;

class Model {
public:

	glm::mat4 tranform=glm::mat4(1.0f);

	Model();
	Model(SHAPE shape,glm::vec3 color);

	void Draw(Camera& camera);
	void Draw(Shader& shader, Camera& camera);

private:
	vector<Mesh*> meshes;
	vector<Texture> texturesLoaded;
};