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
    -0.5f,  -0.5f,  0.5f,
    0.5f,   -0.5f,  0.5f,
    0.5f,   -0.5f,  -0.5f,
    -0.5f,  -0.5f,  -0.5f,
    -0.5f,  0.5f,   0.5f,
    0.5f,   0.5f,   0.5f,
    0.5f,   0.5f,   -0.5f,
    -0.5f,  0.5f,   -0.5f,
};

// 12 triangles of 6 face
static unsigned int CubeIndic[] = {
    0,1,4,  1,5,4,
    1,2,5,  2,6,5,
    2,3,6,  3,7,6,
    3,0,7,  0,4,7,
    4,5,7,  5,6,7,
    2,3,1,  3,0,1,
};




SimpleMesh::SimpleMesh(SHAPE shape){
    if (shape == CUBE) {
        for (int i = 0; i < 8; ++i) {
            Vertex v;
            v.Position[0] = CubeVert[i * 3];
            v.Position[1] = CubeVert[i * 3+1];
            v.Position[2] = CubeVert[i * 3+2];
            vertices.emplace_back(v);
        }
        for (int i = 0; i < 36; ++i) {
            indices.emplace_back(CubeIndic[i]);
        }

    }
    setupMesh();
}

void SimpleMesh::Draw(){

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SimpleMesh::setupMesh(){

    //init VBO,VAO,EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices), &indices[0], GL_STATIC_DRAW);

    //explain data buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}






