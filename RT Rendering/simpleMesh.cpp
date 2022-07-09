#include "simpleMesh.h"

/*

      7---------6
     /|        /|
    4---------5 |
    | |       | |                      y|
    | 3-------|-2                       |____x
    |/        |/                        /
    0---------1                        /z

*/

// 8 vertices of a cube
static float CubeVert[] = {
    // position                 // texture coordinate
    -0.5f,  -0.5f,  0.5f,       0.25f,   0.75f,                 // 0
    0.5f,   -0.5f,  0.5f,       0.5f,  0.75f,                   // 1
    0.5f,   -0.5f,  -0.5f,      0.5f,  0.5f,                    // 2
    -0.5f,  -0.5f,  -0.5f,      0.25f,   0.5f,                  // 3
    -0.5f,  0.5f,   0.5f,       0.25f,   1.0f,                  // 4
    0.5f,   0.5f,   0.5f,       0.5f,  1.0f,                    // 5
    0.5f,   0.5f,   -0.5f,      0.5f,  0.25f,                   // 6
    -0.5f,  0.5f,   -0.5f,      0.25f,   0.25f,                 // 7

    -0.5f,  0.5f,   0.5f,       0.0f,  0.75f,                   //4-1   8
    -0.5f,  0.5f,   0.5f,       0.25f,   0.0f,                  //4-2   9
    0.5f,   0.5f,   0.5f,       0.75f,   0.75f,                 // 5-1  10
    0.5f,   0.5f,   0.5f,       0.5f,  0.0f,                    // 5-2  11

    0.5f,   0.5f,   -0.5f,      0.75f,   0.5f,                  // 6-1  12
    -0.5f,  0.5f,   -0.5f,      0.0f,  0.5f,                    // 7-1  13
};

// 12 triangles of 6 face
static unsigned int CubeIndic[] = {
    0,1,4,  1,5,4,
    1,2,10,  2,12,10,
    2,3,6,  3,7,6,
    3,0,13,  0,8,13,
    9,11,7,  11,6,7,
    2,3,1,  3,0,1,
};




SimpleMesh::SimpleMesh(){
    
    for (int i = 0; i < 14; ++i) {
        Vertex v;
        v.Position = glm::vec3(CubeVert[i * 5], CubeVert[i * 5 + 1], CubeVert[i * 5 + 2]);
        v.TexCoord = glm::vec2(CubeVert[i * 5 + 3], CubeVert[i * 5 + 4]);
        Vertices.emplace_back(v);
    }
    for (int i = 0; i < 36; ++i) {
        Indices.emplace_back(CubeIndic[i]);
    }

        

    // generate and bind texture
    unsigned int texture_id;
    stbi_set_flip_vertically_on_load(true); // flip y-axis
    int width, height, nrChannels;
    unsigned char* data = stbi_load("./Resource/dice.png", &width, &height, &nrChannels, 0);
    if (!data) std::cout << "Failed to load texture" << std::endl;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free data after bind texture
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Texture texture;
    texture.id = texture_id;
    texture.type = TEXTURE_TYPE::DIFFUSE;

    Textures.emplace_back(texture);

    
    setupMesh();
}

SimpleMesh::SimpleMesh(SHAPE shape, glm::vec3 color): Color(color) {

    if (shape == CUBE) {
        for (int i = 0; i < 14; ++i) {
            Vertex v;
            v.Position = glm::vec3(CubeVert[i * 5], CubeVert[i * 5 + 1], CubeVert[i * 5 + 2]);
            v.TexCoord = glm::vec2(CubeVert[i * 5 + 3], CubeVert[i * 5 + 4]);
            Vertices.emplace_back(v);
        }
        for (int i = 0; i < 36; ++i) {
            Indices.emplace_back(CubeIndic[i]);
        }

    }
    setupMesh();

    DefaultShader = Shader("Shaders/pure.vs", "Shaders/pure.fs");

}

void SimpleMesh::Draw(Camera& camera, glm::mat4 parent_trans){
    DefaultShader.use();
    DefaultShader.setVec3("pure_color", Color);

    DefaultShader.setMat4("model_sp", parent_trans * Tranform);
    DefaultShader.setMat4("view_sp", camera.GetViewMatrix());
    DefaultShader.setMat4("proj_sp", camera.GetProjMatrix());


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}




