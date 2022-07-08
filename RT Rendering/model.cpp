#include "model.h"

Model::Model(){
	meshes.emplace_back(SimpleMesh());
}

void Model::Draw(Shader& shader){
	for (Mesh mesh : meshes) {
		mesh.Draw(shader);
	}
}
