#include"camera.h"

Camera::Camera(float height, float width):Height(height),Width(width) {
	UpdateVector();
}

glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjMatrix(){
	if (type == ORTHO) {
		return glm::ortho(0.0f, Width, 0.0f, Height, Near, Far);
	}
	else return glm::perspective(glm::radians(FOV), Width / Height, Near, Far);
}

void Camera::Move(CAMERAMOVE moveDir, float deltaTime){

	if (moveDir == CAMERAMOVE::LEFT) {
		Position -= deltaTime * MoveSpeed * Right;
		return;
	}
	if (moveDir == CAMERAMOVE::RIGHT) {
		Position += deltaTime * MoveSpeed * Right;
		return;
	}
	if (moveDir == CAMERAMOVE::FORWARD) {
		Position += deltaTime * MoveSpeed * Front;
		return;
	}
	if (moveDir == CAMERAMOVE::BACKWARD) {
		Position -= deltaTime * MoveSpeed * Front;
		return;
	}
	if (moveDir == CAMERAMOVE::WDUP) {
		Position += deltaTime * MoveSpeed * WorldUp;
		return;
	}
	if (moveDir == CAMERAMOVE::WDDOWN) {
		Position -= deltaTime * MoveSpeed * WorldUp;
		return;
	}
}

void Camera::Roate(float xoffset, float yoffset){
	Yaw += xoffset * RoateSpeedX;
	Pitch -= yoffset * RoateSpeedY;

	if (Pitch > 89.0f) Pitch = 89.0f;
	if (Pitch < -89.0f) Pitch = -89.0f;

	UpdateVector();
}

void Camera::ScaleFOV(float yoffset){
	FOV -= (float)yoffset;
	if (FOV < 1.0f)
		FOV = 1.0f;
	if (FOV > 60.0f)
		FOV = 60.0f;
}

void Camera::UpdateVector(){

	// updata Right with Yaw

	// calculate the new Front vector directly
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));

}
