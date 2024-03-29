#include "render.h"

void ForwardRender::RenderSet(){

    // screen buffer
    glGenFramebuffers(1, &screenBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, screenBuffer);
    // screen texture
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // bind screen texture to screen buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    // screen texture - blooming
    glGenTextures(1, &screenBloomTexture);
    glBindTexture(GL_TEXTURE_2D, screenBloomTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, screenBloomTexture, 0);
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    // use Renderbuffer Object to get depth and stencil
    glGenRenderbuffers(1, &screenRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, screenRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // bind the Renderbuffer Object to screen buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenRBO);

    // check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Screen Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // MS buffer/texture for MSAA
    glGenFramebuffers(1, &msBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, msBuffer);

    glGenTextures(1, &msTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_samples, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msTexture, 0);

    glGenTextures(1, &msBloomTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msBloomTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_samples, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, msBloomTexture, 0);
    //unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    glGenRenderbuffers(1, &msRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, msRBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa_samples, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: MS Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // buffers for GaussianBlur on bloom texture
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (GLuint i = 0; i < 2; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
        );
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }

    this->screenMesh = make_shared<SimpleMesh>(SHAPE::RECT);
    this->screenMesh->RemoveTextures();
    this->screenShader = new Shader("Shaders/Postprocess/default.vert", "Shaders/Postprocess/default.frag");
    this->shaderBlur = new Shader("Shaders/Postprocess/gaussianBlur.vert", "Shaders/Postprocess/gaussianBlur.frag");


}

void ForwardRender::RenderDraw(Scene* scene){

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


    // first pass
    {
        // bind frame buffer
        if (this->use_MSAA) glBindFramebuffer(GL_FRAMEBUFFER, msBuffer);
        else glBindFramebuffer(GL_FRAMEBUFFER, screenBuffer);
        glDrawBuffers(2, attachments);

        // gl settings and clear
        glDisable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_CULL_FACE);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // set shaders
        scene->setShaderLight(scene->mainShader);
        scene->setShaderCamera(scene->mainShader);
        scene->setShaderEnv(scene->mainShader);
        scene->setShaderOutline();

        // stencil settings for draw outline
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // draw main models
        scene->Draw();
        glStencilMask(0x00);


        // sign for lights
        scene->DrawLights();

        // skybox
        glDepthFunc(GL_LEQUAL);
        scene->DrawSkybox();
        glDepthFunc(GL_LESS);

        // draw outline
        if (this->draw_outline) {

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);

            scene->DrawOutline();

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF); // this will also affect glClear
            glEnable(GL_DEPTH_TEST);
        }
    }

    // msaa
    if (this->use_MSAA) {

        // MS buffer to screen buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, msBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenBuffer);

        glReadBuffer(attachments[0]);
        glDrawBuffer(attachments[0]);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glReadBuffer(attachments[1]);
        glDrawBuffer(attachments[1]);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glDrawBuffers(2, attachments);
    }

    // GaussianBlur on bloom texture
    if (this->use_Bloom) {
        GLboolean first_iteration = true;
        GLuint iter = this->bloom_blur_iter;
        shaderBlur->use();

        for (GLuint i = 0; i < iter; i++) {

            glBindFramebuffer(GL_READ_FRAMEBUFFER, pingpongFBO[0]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingpongFBO[1]);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? screenBloomTexture : pingpongBuffer[0]);
            shaderBlur->setBool("horizontal", true);
            screenMesh->Draw(shaderBlur);

            glBindFramebuffer(GL_READ_FRAMEBUFFER, pingpongFBO[1]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingpongFBO[0]);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[1]);
            shaderBlur->setBool("horizontal", false);
            screenMesh->Draw(shaderBlur);


            if (first_iteration)
                first_iteration = false;

        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // second pass
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glEnable(GL_FRAMEBUFFER_SRGB);
        glDisable(GL_DEPTH_TEST);

        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        screenShader->use();
        screenShader->setBool("useBloom", this->use_Bloom);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        screenShader->setInt("screenTexture", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]);
        screenShader->setInt("screenBloomTexture", 1);
        screenMesh->Draw(screenShader);
    }


}

ForwardRender::~ForwardRender(){
    delete screenShader;
    delete shaderBlur;
}


void DeferredRender::RenderSet(){

    
    // G-buffer
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    // albedo texture
    glGenTextures(1, &g_albedo);
    glBindTexture(GL_TEXTURE_2D, g_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[0], GL_TEXTURE_2D, g_albedo, 0);
    // normal texture
    glGenTextures(1, &g_normal);
    glBindTexture(GL_TEXTURE_2D, g_normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[1], GL_TEXTURE_2D, g_normal, 0);
    // position texture
    glGenTextures(1, &g_position);
    glBindTexture(GL_TEXTURE_2D, g_position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[2], GL_TEXTURE_2D, g_position, 0);
    // material texture
    glGenTextures(1, &g_material);
    glBindTexture(GL_TEXTURE_2D, g_material);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[3], GL_TEXTURE_2D, g_material, 0);

    glDrawBuffers(4, attachments);

    // use Renderbuffer Object to get depth and stencil
    glGenRenderbuffers(1, &gRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, gRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // bind the Renderbuffer Object to screen buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBO);

    // check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Screen Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // screen buffer
    glGenFramebuffers(1, &screenBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, screenBuffer);
    unsigned int screen_attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    // screen texture
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, screen_attachments[0], GL_TEXTURE_2D, screenTexture, 0);
    // screen texture - blooming
    glGenTextures(1, &screenBloomTexture);
    glBindTexture(GL_TEXTURE_2D, screenBloomTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, screen_attachments[1], GL_TEXTURE_2D, screenBloomTexture, 0);

    glDrawBuffers(2, screen_attachments);

    // use Renderbuffer Object to get depth and stencil
    glGenRenderbuffers(1, &screenRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, screenRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenRBO);

    // check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Screen Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // buffers for GaussianBlur on bloom texture
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffer);
    for (GLuint i = 0; i < 2; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
        );
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }


    this->screenMesh = make_shared<SimpleMesh>(SHAPE::RECT);
    this->screenMesh->RemoveTextures();
    this->second_pass_shader = new Shader("Shaders/DeferredRender/PBR_second_pass.vert", "Shaders/DeferredRender/PBR_second_pass.frag");
    this->screenShader = new Shader("Shaders/Postprocess/default.vert", "Shaders/Postprocess/default.frag");
    this->shaderBlur = new Shader("Shaders/Postprocess/gaussianBlur.vert", "Shaders/Postprocess/gaussianBlur.frag");

}

void DeferredRender::RenderDraw(Scene* scene){

    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    unsigned int screen_attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // first pass
    {
        // bind frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glDrawBuffers(4, attachments);

        // gl set and clear
        glDisable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_CULL_FACE);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // set shaders
        scene->setShaderCamera(scene->mainShader);

        // stencil settings for draw outline
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // draw main models
        scene->Draw();
        glStencilMask(0x00);

    }

    // second pass
    {
        glBindFramebuffer(GL_FRAMEBUFFER, screenBuffer);
        glDrawBuffers(2, screen_attachments);

        // gl settings and clear
        glDisable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // set shader
        scene->setShaderLight(this->second_pass_shader);
        scene->setShaderCamera(this->second_pass_shader);
        scene->setShaderEnv(this->second_pass_shader);


        second_pass_shader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_albedo);
        second_pass_shader->setInt("albedo_texture", 0);
        glActiveTexture(GL_TEXTURE0+1);
        glBindTexture(GL_TEXTURE_2D, g_normal);
        second_pass_shader->setInt("normal_texture", 1);
        glActiveTexture(GL_TEXTURE0+2);
        glBindTexture(GL_TEXTURE_2D, g_position);
        second_pass_shader->setInt("position_texture", 2);
        glActiveTexture(GL_TEXTURE0+3);
        glBindTexture(GL_TEXTURE_2D, g_material);
        second_pass_shader->setInt("metallic_roughness_texture", 3);

        screenMesh->Draw(second_pass_shader);

    }

    // GaussianBlur on bloom texture
    if (this->use_Bloom) {
        GLboolean first_iteration = true;
        GLuint iter = this->bloom_blur_iter;
        shaderBlur->use();

        for (GLuint i = 0; i < iter; i++) {

            glBindFramebuffer(GL_READ_FRAMEBUFFER, pingpongFBO[0]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingpongFBO[1]);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? screenBloomTexture : pingpongBuffer[0]);
            shaderBlur->setBool("horizontal", true);
            screenMesh->Draw(shaderBlur);

            glBindFramebuffer(GL_READ_FRAMEBUFFER, pingpongFBO[1]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingpongFBO[0]);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[1]);
            shaderBlur->setBool("horizontal", false);
            screenMesh->Draw(shaderBlur);

            if (first_iteration)
                first_iteration = false;
        }

    }


    // additional forward pass for postprocess and other rendering
    {
        // rendering main models from screen buffer to default buffer
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glEnable(GL_FRAMEBUFFER_SRGB);
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            screenShader->use();
            screenShader->setBool("useBloom", this->use_Bloom);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screenTexture);
            screenShader->setInt("screenTexture", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]);
            screenShader->setInt("screenBloomTexture", 1);
            screenMesh->Draw(screenShader);
        }

        // copy depth and stencil buffer from first pass
        glEnable(GL_DEPTH_TEST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // set shader
        scene->setShaderOutline();

        // sign for lights
        scene->DrawLights();

        // skybox
        glDepthFunc(GL_LEQUAL);
        scene->DrawSkybox();
        glDepthFunc(GL_LESS);

        // draw outline
        if (this->draw_outline) {

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);

            scene->DrawOutline();

            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF); // this will also affect glClear
            glEnable(GL_DEPTH_TEST);
        }

    }


}

DeferredRender::~DeferredRender(){
    delete second_pass_shader;
    delete screenShader;
    delete shaderBlur;
}
