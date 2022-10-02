#include "light.h"
#include<iostream>

PointLight::PointLight(){

	DrawShader = std::make_shared<Shader>("Shaders/BasicShader/lightDraw.vert", "Shaders/BasicShader/lightDraw.frag");

	int vn = 24;
	float r = 0.02f;
	float* vertices = new float[vn*3];

	for (int i = 0; i < vn; ++i) {
		vertices[i*3] = glm::cos(glm::radians((float)360 / vn * i)) * r;
		vertices[i*3+1] = glm::sin(glm::radians((float)360 / vn * i)) * r;
		vertices[i*3 + 2] = 0.0f;
	}

	glGenVertexArrays(1, &DrawVAO);
	glGenBuffers(1, &DrawVBO);

	glBindVertexArray(DrawVAO);
	glBindBuffer(GL_ARRAY_BUFFER, DrawVBO);
	glBufferData(GL_ARRAY_BUFFER, 3*vn*sizeof(float), vertices, GL_STATIC_DRAW);
		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

PointLight::PointLight(std::shared_ptr<Shader> DrawShader){

	this->DrawShader = DrawShader;

	int vn = 24;
	float r = 0.02f;
	float* vertices = new float[vn * 3];

	for (int i = 0; i < vn; ++i) {
		vertices[i * 3] = glm::cos(glm::radians((float)360 / vn * i)) * r;
		vertices[i * 3 + 1] = glm::sin(glm::radians((float)360 / vn * i)) * r;
		vertices[i * 3 + 2] = 0.0f;
	}

	glGenVertexArrays(1, &DrawVAO);
	glGenBuffers(1, &DrawVBO);

	glBindVertexArray(DrawVAO);
	glBindBuffer(GL_ARRAY_BUFFER, DrawVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * vn * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void PointLight::Draw(Camera& camera) {

	DrawShader->use();

	// billboard effect by roate the Transform
	glm::vec3 trans = glm::vec3(Transform[3][0], Transform[3][1], Transform[3][2]);
	glm::mat4 new_transform = glm::translate(glm::mat4(1.0f), -trans) * Transform;
	glm::mat4 rot = camera.GetViewMatrix();
	rot[3][0] = 0;
	rot[3][1] = 0;
	rot[3][2] = 0;
	new_transform = glm::inverse(rot) * new_transform;
	new_transform = glm::translate(glm::mat4(1.0f), trans) * new_transform;

	DrawShader->setMat4("model_sp", new_transform);
	DrawShader->setMat4("view_sp", camera.GetViewMatrix());
	DrawShader->setMat4("proj_sp", camera.GetProjMatrix());


	glBindVertexArray(DrawVAO);
	glDrawArrays(GL_LINES, 0, 24);

}

DirLight::DirLight() {

	DrawShader = std::make_shared<Shader>("Shaders/BasicShader/lightDraw.vert", "Shaders/BasicShader/lightDraw.frag");

	float vertices[18] = {
		0.03f,0.05f,0.0f,	0.03f,-0.05f,0.0f,
		0.0f,0.05f,0.0f,	0.0f,-0.05f,0.0f,
		-0.03f,0.05f,0.0f,	-0.03f,-0.05f,0.0f,
	};

	glGenVertexArrays(1, &DrawVAO);
	glGenBuffers(1, &DrawVBO);

	glBindVertexArray(DrawVAO);
	glBindBuffer(GL_ARRAY_BUFFER, DrawVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// make the mark displayed at top-right defaulted.
	Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.0f));

}

DirLight::DirLight(std::shared_ptr<Shader> DrawShader){

	this->DrawShader = DrawShader;

	float vertices[18] = {
	0.03f,0.05f,0.0f,	0.03f,-0.05f,0.0f,
	0.0f,0.05f,0.0f,	0.0f,-0.05f,0.0f,
	-0.03f,0.05f,0.0f,	-0.03f,-0.05f,0.0f,
	};

	glGenVertexArrays(1, &DrawVAO);
	glGenBuffers(1, &DrawVBO);

	glBindVertexArray(DrawVAO);
	glBindBuffer(GL_ARRAY_BUFFER, DrawVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// make the mark displayed at top-right defaulted.
	Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.0f));
}

void DirLight::Draw(Camera& camera) {

	DrawShader->use();

	DrawShader->setMat4("model_sp", Transform);
	DrawShader->setMat4("view_sp", camera.GetViewMatrix());
	DrawShader->setMat4("proj_sp", camera.GetProjMatrix());

	glBindVertexArray(DrawVAO);
	glDrawArrays(GL_LINES, 0, 6);
}

glm::vec3 DirLight::GetDirection(){
	glm::mat4 rot = Transform;
	rot[3][0] = 0;
	rot[3][1] = 0;
	rot[3][2] = 0;

	return glm::vec3(rot * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

}

SpotLight::SpotLight(){

	DrawShader = std::make_shared<Shader>("Shaders/BasicShader/lightDraw.vert", "Shaders/BasicShader/lightDraw.frag");

	float vertices[24] = {
		0.0f,0.05f,0.0f,	-0.02f,0.0f,0.0f,
		-0.02f,0.0f,0.0f,	0.02f,0.0f,0.0f,
		0.02f,0.0f,0.0f,	0.0f,0.05f,0.0f,
		0.0f,0.05f,0.0f,	0.0f,-0.05f,0.0f,
	};

	glGenVertexArrays(1, &DrawVAO);
	glGenBuffers(1, &DrawVBO);

	glBindVertexArray(DrawVAO);
	glBindBuffer(GL_ARRAY_BUFFER, DrawVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

SpotLight::SpotLight(std::shared_ptr<Shader> DrawShader){

	this->DrawShader = DrawShader;

	float vertices[24] = {
		0.0f,0.05f,0.0f,	-0.02f,0.0f,0.0f,
		-0.02f,0.0f,0.0f,	0.02f,0.0f,0.0f,
		0.02f,0.0f,0.0f,	0.0f,0.05f,0.0f,
		0.0f,0.05f,0.0f,	0.0f,-0.05f,0.0f,
	};

	glGenVertexArrays(1, &DrawVAO);
	glGenBuffers(1, &DrawVBO);

	glBindVertexArray(DrawVAO);
	glBindBuffer(GL_ARRAY_BUFFER, DrawVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpotLight::Draw(Camera& camera){

	DrawShader->use();

	DrawShader->setMat4("model_sp", Transform);
	DrawShader->setMat4("view_sp", camera.GetViewMatrix());
	DrawShader->setMat4("proj_sp", camera.GetProjMatrix());

	glBindVertexArray(DrawVAO);
	glDrawArrays(GL_LINES, 0, 8);

}

glm::vec3 SpotLight::GetDirection(){

	glm::mat4 rot = Transform;
	rot[3][0] = 0;
	rot[3][1] = 0;
	rot[3][2] = 0;

	return glm::vec3(rot * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
}

Light::~Light(){

	glDeleteVertexArrays(1, &DrawVAO);
	glDeleteBuffers(1, &DrawVBO);

}
