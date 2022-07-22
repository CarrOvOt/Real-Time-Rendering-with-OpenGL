#include "model.h"

Model::Model(){
	meshes.emplace_back(new SimpleMesh());
}

Model::Model(SHAPE shape){
	meshes.emplace_back(new SimpleMesh(shape));
}


void Model::Draw(Shader& shader, Camera& camera){
	for (Mesh* mesh : meshes) {
		mesh->Draw(shader, camera, transform);
	}
}
