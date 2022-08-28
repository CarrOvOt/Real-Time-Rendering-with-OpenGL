#pragma once

#include <vector>

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"

//#include "utility/IBL.h" // will lead re-define, why?

using std::vector;

class Scene {

public:

	// models
	vector<Model> Models;

	// shader
	Shader mainShader;
	Shader outlineShader;

	// direct lights
	vector<SpotLight> SpotLights;
	vector<DirLight> DirLights;
	vector<PointLight> PointLights;

	// camera for output 
	Camera mainCamera;
	
	// envs
	Skybox mainSkybox;

	// envs for PBR
	unsigned int preFilteredCubemap;
	unsigned int irradianceCubemap;
	unsigned int brdfLut;

	// others
	float outline_width=0.05f;
	glm::vec3 outline_color = glm::vec3(0.1f, 0.4f, 0.2f);




	// set IBL textures for PBR
	void setIBL();

	// set uniform vars in shader
	void setShaderLight();
	void setShaderCamera();
	void setShaderEnv();
	void setShaderOutline();

	// draw model, lights sign, skybox, outlines
	void Draw();
	void DrawLights();
	void DrawSkybox();
	void DrawOutline();

};