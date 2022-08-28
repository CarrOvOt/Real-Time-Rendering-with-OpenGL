#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum CAMERATYPE{
	ORTHO,PERSPECTIVE
};

enum CAMERAMOVE {
	LEFT,RIGHT,FORWARD,BACKWARD,
	WDUP, WDDOWN // up and down in world space
};

const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);


class Camera {

public:

	CAMERATYPE type = PERSPECTIVE;


	float Height;
	float Width;
	float FOV = 45.0f;
	float Near=0.1f;
	float Far=100.0f;

	float Pitch = 0;
	float Yaw = -90.0f;

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);

	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);


	float MoveSpeed = 2.0f;
	float RoateSpeedX = 0.1f;
	float RoateSpeedY = 0.1f;
	float ScaleSpeed = 2.0f;


	Camera();
	Camera(float height,float width);


	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjMatrix();

	// only affect Postion
	void Move(CAMERAMOVE moveDir,float deltaTime);
		
	// only affect Pitch and Yaw
	void Roate(float xoffset, float yoffset);

	void ScaleFOV(float yoffset);

	
private:

	// updata Front,Up,Right with Pictch and Yaw
	void UpdateVector();


};