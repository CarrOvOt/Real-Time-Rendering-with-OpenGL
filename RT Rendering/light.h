#pragma once

#include <glm/glm.hpp>

#include "shader.h"
#include "camera.h"

class Light {
public:
	glm::mat4 Transform = glm::mat4(1.0f);

	float Power = 1.0f;
	
	glm::vec3 ambient = glm::vec3(0.1f);
	glm::vec3 diffuse = glm::vec3(0.3f);
	glm::vec3 specular = glm::vec3(0.6f);

	void virtual Draw(Camera& camera)=0;

protected:

	unsigned int DrawVAO, DrawVBO;
	Shader DrawShader;

};

class PointLight : public Light {
public:
	PointLight();
	void Draw(Camera& camera) override;

private:

};

class DirLight : public Light {
public:
	DirLight();
	void Draw(Camera& camera) override;
	glm::vec3 GetDirection();// (0,-1,0) is the defaut direction without any transform
};

class SpotLight : public Light {
public:
	float R1 = 20.0f; // Inner Angle
	float R2 = 25.0f; // Outer Angle

	SpotLight();
	void Draw(Camera& camera) override;
	glm::vec3 GetDirection();// (0,-1,0) is the defaut direction without any transform
};