#include "mesh.h"

Mesh::Mesh(){
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
    this->Vertices = vertices;
    this->Indices = indices;
    this->Textures = textures;
    setupMesh();
}



void Mesh::Draw(Shader& shader, Camera& camera, glm::mat4 parent_trans){

    shader.use();

    shader.setMat4("model_sp", parent_trans*Transform);
    shader.setMat4("view_sp", camera.GetViewMatrix());
    shader.setMat4("proj_sp", camera.GetProjMatrix());

	// bind textures
	unsigned int diffuseNr = 1, specularNr = 1, normalNr = 1, metallicNr = 1, roughNr = 1;

	for (unsigned int i = 0; i < Textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		string name;
		if (Textures[i].type == DIFFUSE) {
			name = "texture_diffuse" + std::to_string(diffuseNr++);
		}
        if (Textures[i].type == SPECULAR) {
            name = "texture_specular" + std::to_string(specularNr++);
        }
        if (Textures[i].type == NORMAL) {
            name = "texture_normal" + std::to_string(normalNr++);
        }
        if (Textures[i].type == METALLIC) {
            name = "texture_metallic" + std::to_string(metallicNr++);
        }
        if (Textures[i].type == ROUGHNESS) {
            name = "texture_roughness" + std::to_string(roughNr++);
        }
		shader.setInt(name.c_str(), i);
		glBindTexture(GL_TEXTURE_2D, Textures[i].id);
	}


	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);

}

void Mesh::Draw(Shader& shader){
    shader.use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::setupMesh(){

    //init VBO,VAO,EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    //explain data buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

}
