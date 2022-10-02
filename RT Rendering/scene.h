#pragma once

#include <vector>
#include <memory>

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"

//#include "utility/IBL.h" // will lead re-define, why?

using std::vector;
using std::shared_ptr;

class Scene {

public:

	// models
	vector<shared_ptr<Model>> Models;

	// shader
	Shader* mainShader = NULL;
	Shader* outlineShader = NULL;

	// direct lights
	vector<shared_ptr<SpotLight>> SpotLights;
	vector<shared_ptr<DirLight>> DirLights;
	vector<shared_ptr<PointLight>> PointLights;

	// camera for output 
	Camera mainCamera;
	
	// envs
	Skybox* mainSkybox = NULL;

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
	void setShaderLight(Shader* shader);
	void setShaderCamera(Shader* shader);
	void setShaderEnv(Shader* shader);
	void setShaderOutline();

	// draw model, lights sign, skybox, outlines
	void Draw();
	void DrawLights();
	void DrawSkybox();
	void DrawOutline();


	~Scene();
};