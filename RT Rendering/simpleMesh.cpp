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
    // position                 // normal               // texture coordinate  

    -0.5f,  -0.5f,  0.5f,       0.0f,0.0f,1.0f,         0.25f,  0.75f,  // 0
    0.5f,   -0.5f,  0.5f,       0.0f,0.0f,1.0f,         0.5f,   0.75f,  // 1
    -0.5f,  0.5f,   0.5f,       0.0f,0.0f,1.0f,         0.25f,  1.0f,   // 4
    0.5f,   -0.5f,  0.5f,       0.0f,0.0f,1.0f,         0.5f,   0.75f,  // 1
    0.5f,   0.5f,   0.5f,       0.0f,0.0f,1.0f,         0.5f,   1.0f,   // 5
    -0.5f,  0.5f,   0.5f,       0.0f,0.0f,1.0f,         0.25f,  1.0f,   // 4

    0.5f,   -0.5f,  0.5f,       1.0f,0.0f,0.0f,         0.5f,   0.75f,  // 1
    0.5f,   -0.5f,  -0.5f,      1.0f,0.0f,0.0f,         0.5f,   0.5f,   // 2
    0.5f,   0.5f,   0.5f,       1.0f,0.0f,0.0f,         0.75f,  0.75f,  // 5-1
    0.5f,   -0.5f,  -0.5f,      1.0f,0.0f,0.0f,         0.5f,   0.5f,   // 2
    0.5f,   0.5f,   -0.5f,      1.0f,0.0f,0.0f,         0.75f,  0.5f,   // 6-1 
    0.5f,   0.5f,   0.5f,       1.0f,0.0f,0.0f,         0.75f,  0.75f,  // 5-1 

    0.5f,   -0.5f,  -0.5f,      0.0f,0.0f,-1.0f,        0.5f,   0.5f,   // 2
    -0.5f,  -0.5f,  -0.5f,      0.0f,0.0f,-1.0f,        0.25f,  0.5f,   // 3
    0.5f,   0.5f,   -0.5f,      0.0f,0.0f,-1.0f,        0.5f,   0.25f,  // 6
    -0.5f,  -0.5f,  -0.5f,      0.0f,0.0f,-1.0f,        0.25f,  0.5f,   // 3
    -0.5f,  0.5f,   -0.5f,      0.0f,0.0f,-1.0f,        0.25f,  0.25f,  // 7
    0.5f,   0.5f,   -0.5f,      0.0f,0.0f,-1.0f,        0.5f,   0.25f,  // 6

    -0.5f,  -0.5f,  -0.5f,      -1.0f,0.0f,0.0f,         0.25f,  0.5f,   // 3
    -0.5f,  -0.5f,  0.5f,       -1.0f,0.0f,0.0f,         0.25f,  0.75f,  // 0
    -0.5f,  0.5f,   -0.5f,      -1.0f,0.0f,0.0f,         0.0f,   0.5f,   // 7-1 
    -0.5f,  -0.5f,  0.5f,       -1.0f,0.0f,0.0f,         0.25f,  0.75f,  // 0
    -0.5f,  0.5f,   0.5f,       -1.0f,0.0f,0.0f,         0.0f,   0.75f,  // 4-1
    -0.5f,  0.5f,   -0.5f,      -1.0f,0.0f,0.0f,         0.0f,   0.5f,   // 7-1 

    -0.5f,  0.5f,   0.5f,       0.0f,1.0f,0.0f,         0.25f,  0.0f,   // 4-2 
    0.5f,   0.5f,   0.5f,       0.0f,1.0f,0.0f,         0.5f,   0.0f,   // 5-2 
    -0.5f,  0.5f,   -0.5f,      0.0f,1.0f,0.0f,         0.25f,  0.25f,  // 7
    0.5f,   0.5f,   0.5f,       0.0f,1.0f,0.0f,         0.5f,   0.0f,   // 5-2 
    0.5f,   0.5f,   -0.5f,      0.0f,1.0f,0.0f,         0.5f,   0.25f,  // 6
    -0.5f,  0.5f,   -0.5f,      0.0f,1.0f,0.0f,         0.25f,  0.25f,  // 7

    -0.5f,  -0.5f,  0.5f,       0.0f,-1.0f,0.0f,        0.25f,  0.75f,  // 0
    -0.5f,  -0.5f,  -0.5f,      0.0f,-1.0f,0.0f,        0.25f,  0.5f,   // 3
    0.5f,   -0.5f,  0.5f,       0.0f,-1.0f,0.0f,        0.5f,   0.75f,  // 1
    -0.5f,  -0.5f,  -0.5f,      0.0f,-1.0f,0.0f,        0.25f,  0.5f,   // 3
    0.5f,   -0.5f,  -0.5f,      0.0f,-1.0f,0.0f,        0.5f,   0.5f,   // 2
    0.5f,   -0.5f,  0.5f,       0.0f,-1.0f,0.0f,        0.5f,   0.75f,  // 1
};

// 12 triangles of 6 face
static unsigned int CubeIndic[] = {
    0,1,2,  3,4,5,
    6,7,8,  9,10,11,
    12,13,14,  15,16,17,
    18,19,20,  21,22,23,
    24,25,26,  27,28,29,
    30,31,32,  33,34,35,
};




SimpleMesh::SimpleMesh(){
    
    for (int i = 0; i < 36; ++i) {
        Vertex v;
        v.Position = glm::vec3(CubeVert[i * 8], CubeVert[i * 8 + 1], CubeVert[i * 8 + 2]);
        v.Normal = glm::vec3(CubeVert[i * 8+3], CubeVert[i * 8 + 4], CubeVert[i * 8 + 5]);
        v.TexCoord = glm::vec2(CubeVert[i * 8 + 6], CubeVert[i * 8 + 7]);
        Vertices.emplace_back(v);
    }
    for (int i = 0; i < 36; ++i) {
        Indices.emplace_back(CubeIndic[i]);
    }

    Textures.emplace_back(loadTexture("./Resource/09diffuse.png",TEXTURE_TYPE::DIFFUSE));
    Textures.emplace_back(loadTexture("./Resource/09specular.png", TEXTURE_TYPE::SPECULAR));

    
    setupMesh();
}

SimpleMesh::SimpleMesh(SHAPE shape, glm::vec3 color): Color(color) {

    if (shape == CUBE) {
        for (int i = 0; i < 36; ++i) {
            Vertex v;
            v.Position = glm::vec3(CubeVert[i * 8], CubeVert[i * 8 + 1], CubeVert[i * 8 + 2]);
            v.Normal = glm::vec3(CubeVert[i * 8 + 3], CubeVert[i * 8 + 4], CubeVert[i * 8 + 5]);
            v.TexCoord = glm::vec2(CubeVert[i * 8 + 6], CubeVert[i * 8 + 7]);
            Vertices.emplace_back(v);
        }
        for (int i = 0; i < 36; ++i) {
            Indices.emplace_back(CubeIndic[i]);
        }

    }
    setupMesh();

    DefaultShader = Shader("Shaders/pure.vert", "Shaders/pure.frag");

}

void SimpleMesh::Draw(Camera& camera, glm::mat4 parent_trans){
    DefaultShader.use();
    DefaultShader.setVec3("pure_color", Color);

    DefaultShader.setMat4("model_sp", parent_trans * Transform);
    DefaultShader.setMat4("view_sp", camera.GetViewMatrix());
    DefaultShader.setMat4("proj_sp", camera.GetProjMatrix());


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SimpleMesh::SetColor(glm::vec3 color){
    Color = color;
}

Texture SimpleMesh::loadTexture(string file_path, TEXTURE_TYPE type){
    // generate and bind texture
    unsigned int texture_id;
    stbi_set_flip_vertically_on_load(true); // flip y-axis
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) std::cout << "Failed to load texture" << std::endl;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free data after bind texture
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture{texture_id, type};
}




