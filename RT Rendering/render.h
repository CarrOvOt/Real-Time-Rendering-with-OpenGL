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
	SimpleMesh screenMesh;

	// screen shader
	Shader screenShader;
	// gaussian blur for bloom
	Shader shaderBlur;



	void ForwardRenderSet();
	void ForwardRenderDraw(Scene* scene);

};