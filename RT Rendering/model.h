#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "simpleMesh.h"
#include "camera.h"

#include <string>
#include <memory>

using namespace std;

class Model {
public:

	glm::mat4 transform=glm::mat4(1.0f);

	vector<shared_ptr<Mesh>> meshes;
	vector<Texture> texturesLoaded;

	string directory;

	Model();
	Model(string file);
	Model(SHAPE shape);

	~Model();

	void Draw(Shader* shader, Camera& camera);
	void Draw(Shader* shader);

	void LoadModel(string path);

private:

	void processNode(aiNode* node, const aiScene* scene);
	shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TEXTURE_TYPE typeName);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma=false);


};