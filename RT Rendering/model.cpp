#include "model.h"

Model::Model(){
	meshes.emplace_back(new SimpleMesh());
}

Model::Model(SHAPE shape, glm::vec3 color){
	meshes.emplace_back(new SimpleMesh(shape, color));
}

void Model::Draw(Camera& camera){
	for (Mesh* mesh : meshes) {
		mesh->Draw(camera,transform);
	}
}

void Model::Draw(Shader& shader, Camera& camera){
	for (Mesh* mesh : meshes) {
		mesh->Draw(shader, camera, transform);
	}
}
