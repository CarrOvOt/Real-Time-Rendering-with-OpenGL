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




SimpleMesh::SimpleMesh(SHAPE shape){
    if (shape == CUBE) {
        for (int i = 0; i < 14; ++i) {
            Vertex v;
            v.Position = glm::vec3(CubeVert[i * 5], CubeVert[i * 5 + 1], CubeVert[i * 5 + 2]);
            v.TexCoord = glm::vec2(CubeVert[i * 5 + 3], CubeVert[i * 5 + 4]);
            vertices.emplace_back(v);
        }
        for (int i = 0; i < 36; ++i) {
            indices.emplace_back(CubeIndic[i]);
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

        textures.emplace_back(texture);


    }
    setupMesh();
}

//void SimpleMesh::Draw(Shader& shader){
//
//    glBindVertexArray(VAO);
//    glBindTexture(GL_TEXTURE_2D, texture);
//
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}

//void SimpleMesh::setupMesh(){
//
//    //init VBO,VAO,EBO
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices), &indices[0], GL_STATIC_DRAW);
//
//    //explain data buffer
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
//
//    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    glBindVertexArray(0);
//
//
//    // texture
//    stbi_set_flip_vertically_on_load(true); // flip y-axis
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load("./Resource/dice.png", &width, &height, &nrChannels, 0);
//    if(!data) std::cout<< "Failed to load texture" << std::endl;
//
//    //unsigned int texture;
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    // free data after bind texture
//    stbi_image_free(data);
//
//
//}
//





