#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include "shader.h"
#include "camera.h"

#include <string>
#include <vector>

class Skybox {

public:

	Skybox();
    Skybox(std::string path);

	void Draw(Camera& camera);

    void ReloadCubemap(std::string path);

	
	unsigned int GetTextureID();
	void SetTextureID(unsigned int id);

private:

	Shader shader;

	unsigned int VAO, VBO, texture;

    unsigned int loadCubemap(std::string path);


};