#pragma once

#include "scene.h"


class ForwardRender {

public:

	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;

	unsigned int msaa_samples = 4;
	bool use_MSAA = true;

	bool draw_outline = false;

	bool use_Bloom = false;
	unsigned int bloom_blur_iter = 5;


	// frame buffers, textures
	unsigned int screenBuffer;
	unsigned int screenTexture;
	unsigned int screenBloomTexture;
	unsigned int screenRBO;
	unsigned int msBuffer;
	unsigned int msTexture;
	unsigned int msBloomTexture;
	unsigned int msRBO;

	// for gaussion blur
	GLuint pingpongFBO[2];
	GLuint pingpongBuffer[2];

	// screen mesh
	shared_ptr<SimpleMesh> screenMesh;

	// screen shader
	Shader* screenShader = NULL;

	// gaussian blur for bloom
	Shader* shaderBlur = NULL;


	void RenderSet();
	void RenderDraw(Scene* scene);


	~ForwardRender();

};


class DeferredRender {

public:

	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;

	bool draw_outline = false;

	bool use_Bloom = false;
	unsigned int bloom_blur_iter = 5;

	// screen mesh
	shared_ptr<SimpleMesh> screenMesh;

	// G-buffer
	unsigned int gBuffer;
	unsigned int gRBO;
	unsigned int g_albedo, g_normal, g_position, g_material; // g_material includes metallic and roughness


	// screen buffer and texture for postporcess
	unsigned int screenBuffer;
	unsigned int screenTexture;
	unsigned int screenRBO;
	unsigned int screenBloomTexture;

	// for gaussion blur
	GLuint pingpongFBO[2];
	GLuint pingpongBuffer[2];


	// screen shader
	Shader* second_pass_shader = NULL;
	Shader* screenShader = NULL;

	// gaussian blur for bloom
	Shader* shaderBlur = NULL;


	void RenderSet();
	void RenderDraw(Scene* scene);

	~DeferredRender();

};
