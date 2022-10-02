#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "shader.h"
#include "camera.h"

class Light {
public:
	glm::mat4 Transform = glm::mat4(1.0f);

	float Power = 1.0f;
	
	glm::vec3 ambient = glm::vec3(0.05f);
	glm::vec3 diffuse = glm::vec3(0.3f);
	glm::vec3 specular = glm::vec3(0.65f);

	void virtual Draw(Camera& camera)=0;


	Light() = default;
	Light(const Light&) = delete;

	~Light();

protected:

	unsigned int DrawVAO, DrawVBO;
	std::shared_ptr<Shader> DrawShader;

};

class PointLight : public Light {
public:
	PointLight();
	PointLight(std::shared_ptr<Shader> DrawShader);
	void Draw(Camera& camera) override;

private:

};

class DirLight : public Light {
public:
	DirLight();
	DirLight(std::shared_ptr<Shader> DrawShader);
	void Draw(Camera& camera) override;
	glm::vec3 GetDirection();// (0,-1,0) is the defaut direction without any transform
};

class SpotLight : public Light {
public:
	float R1 = 25.0f; // Inner Angle
	float R2 = 30.0f; // Outer Angle

	SpotLight();
	SpotLight(std::shared_ptr<Shader> DrawShader);
	void Draw(Camera& camera) override;
	glm::vec3 GetDirection();// (0,-1,0) is the defaut direction without any transform
};