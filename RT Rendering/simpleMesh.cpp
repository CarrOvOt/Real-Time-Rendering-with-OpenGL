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

// DO NOT modify this numbers because screen buffer is shown by a Rect SimpleMesh
static float RectVert[] = {
    // position                 // normal           // texture coordinate  
    1.0f,   1.0f,   0.0f,       0.0f,0.0f,1.0f,     1.0f,   1.0f,
    -1.0f,  1.0f,   0.0f,       0.0f,0.0f,1.0f,     0.0f,  1.0f,
    -1.0f,  -1.0f,  0.0f,       0.0f,0.0f,1.0f,     0.0f,  0.0f,
    1.0f,   -1.0f,  0.0f,       0.0f,0.0f,1.0f,     1.0f,   0.0f,
};

static unsigned int RectIndic[] = {
    2,3,0,
    2,0,1,
};



SimpleMesh::SimpleMesh(){
    
}

SimpleMesh::SimpleMesh(SHAPE shape){

    if (shape == RECT) {
        for (int i = 0; i < 4; ++i) {
            Vertex v;
            v.Position = glm::vec3(RectVert[i * 8], RectVert[i * 8 + 1], RectVert[i * 8 + 2]);
            v.Normal = glm::vec3(RectVert[i * 8 + 3], RectVert[i * 8 + 4], RectVert[i * 8 + 5]);
            v.TexCoord = glm::vec2(RectVert[i * 8 + 6], RectVert[i * 8 + 7]);
            Vertices.emplace_back(v);
        }
        for (int i = 0; i < 6; ++i) {
            Indices.emplace_back(RectIndic[i]);
        }

        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_COL_1K.jpg", TEXTURE_TYPE::DIFFUSE));
        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_REFL_1K.jpg", TEXTURE_TYPE::SPECULAR));
        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_NRM_1K.jpg", TEXTURE_TYPE::NORMAL));

        setupTangent();

    }

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

        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_COL_1K.jpg", TEXTURE_TYPE::DIFFUSE));
        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_REFL_1K.jpg", TEXTURE_TYPE::SPECULAR));
        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_NRM_1K.jpg", TEXTURE_TYPE::NORMAL));

        setupTangent();
    }

    if (shape == SPHERE) {

        using glm::vec2;
        using glm::vec3;
        using glm::sin;
        using glm::cos;

        int USEG = 64;
        int VSEG = 64;
        float PI = 3.14159265359f;

        for (int i = 0; i < USEG+1; ++i) {
            for (int j = 0; j < VSEG+1; ++j) {

                float alpha = 2 * PI / USEG * i;
                float beta = PI / VSEG * j - PI / 2;
                Vertex v;
                v.Position = vec3(cos(beta)*sin(alpha),sin(beta), cos(beta) * cos(alpha));
                v.Normal = v.Position;
                v.TexCoord = vec2((float)i/USEG, (float)j/VSEG);

                this->Vertices.push_back(v);
            }
        }

        for (int i = 0; i < USEG; ++i) {
            for (int j = 0; j < VSEG; ++j) {
                if (j != VSEG - 1) {
                    this->Indices.push_back(i * (VSEG +1) + j);
                    this->Indices.push_back((i + 1) * (VSEG + 1) + j + 1);
                    this->Indices.push_back(i * (VSEG + 1) + j + 1);
                }
                if (j != 0) {
                    this->Indices.push_back(i * (VSEG + 1) + j);
                    this->Indices.push_back((i + 1) * (VSEG + 1) + j );
                    this->Indices.push_back((i + 1) * (VSEG + 1) + j + 1);
                }
            }
        }
        
        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_COL_1K.jpg", TEXTURE_TYPE::DIFFUSE));
        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_REFL_1K.jpg", TEXTURE_TYPE::SPECULAR));
        //Textures.emplace_back(loadTexture("./Resource/WoodFlooring/WoodFlooringMahoganyAfricanSanded001_NRM_1K.jpg", TEXTURE_TYPE::NORMAL));

        setupTangent();
    }

    setupMesh();

}

void SimpleMesh::RemoveTextures(){
    this->Textures.clear();
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

    unsigned int color_space = GL_RGB;
    if (type == TEXTURE_TYPE::DIFFUSE) {
        color_space = GL_SRGB;
    }
    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, color_space, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, color_space, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "Texture channel is neither 3 nor 4"<< std::endl;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    // free data after bind texture
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture{texture_id, type, file_path};
}

void SimpleMesh::setupTangent(){

    using glm::vec2;
    using glm::vec3;

    // calculate and average tangents of one vertex that belongs to several triangles
    vector<vector<vec3>> tangents = vector<vector<vec3>>(Vertices.size());
    vector<vector<vec3>> bitangents = vector<vector<vec3>>(Vertices.size());
    for (int i = 0; i < Indices.size();i+=3) {
        vec3 t, b;
        calculateTangent(Vertices[Indices[i]], Vertices[Indices[i + 1]], Vertices[Indices[i + 2]],t,b);

        tangents[Indices[i]].push_back(t);
        tangents[Indices[i+1]].push_back(t);
        tangents[Indices[i+2]].push_back(t);
        bitangents[Indices[i]].push_back(b);
        bitangents[Indices[i + 1]].push_back(b);
        bitangents[Indices[i + 2]].push_back(b);
    }

    for (int i = 0; i < Vertices.size(); ++i) {
        vec3 t = vec3(0.0f);
        vec3 b = vec3(0.0f);
        for (int j = 0; j < tangents[i].size(); ++j) {
            t = t + tangents[i][j];
            b = b + bitangents[i][j];
        }
        Vertices[i].Tangent = glm::normalize(t);
        Vertices[i].Bitangent = glm::normalize(b);
    }

}

void SimpleMesh::calculateTangent(Vertex a, Vertex b, Vertex c, glm::vec3& tangent, glm::vec3& bitangent){

    using glm::vec3;
    using glm::vec2;


    vec3 edge1 = b.Position - a.Position;
    vec3 edge2 = c.Position - a.Position;
    vec2 deltaUV1 = b.TexCoord - a.TexCoord;
    vec2 deltaUV2 = c.TexCoord - a.TexCoord;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = glm::normalize(tangent);

    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent = glm::normalize(bitangent);

}




