// GLFW and GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ImGUI
#include <ImGUI/imgui.h>
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// our code
#include "shader.h"
#include "model.h"
#include "gui.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"

// others
#include <iostream>
#include <string>


// settings
static unsigned int SCR_WIDTH = 1280;
static unsigned int SCR_HEIGHT = 720;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// mouse pos in last frame
float lastMouseX = SCR_WIDTH / 2.0;
float lastMouseY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

// main camera
Camera mainCamera = Camera(SCR_HEIGHT / 200.0f, SCR_WIDTH / 200.0f);

// show mouse to use gui and hide mouse to use camera
bool showMouse = true;

// options & settings in GUI
bool use_wireframe_mode = false;

glm::vec3 background_color = glm::vec3(0.1f, 0.1f, 0.1f );

glm::vec3 rot_xyz = glm::vec3(-90.0f, 90.0f, 90.0f);
float scale_xyz = 1.0f;
float trans_x = 0.0f;


glm::vec3 light_point_pos = glm::vec3(0.0f, 0.65f, -3.0f);
float light_point_power=1.0f;

glm::vec3 light_dir_rot = glm::vec3(0.0f, 0.0f, 0.0f);
float light_dir_power = 1.0f;

glm::vec3 light_spot_pos = glm::vec3(0.3f, 0.3f, 0.3f);
glm::vec3 light_spot_rot = glm::vec3(50.0f, 0.0f, 0.0f);
float light_spot_power = 1.0f;


float shininess= 0.25f;

bool draw_outline = false;

bool use_MSAA = true;
unsigned int msaa_samples = 4;

bool use_Bloom = false;
float bloom_strength = 1.0f;
int bloom_blur = 5;



// rendering gui window
void GUITick() {

    //ImGUI
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Setting");

        if (ImGui::CollapsingHeader("Global Setting")) {

            ImGui::ColorEdit3("background color", (float*)&background_color); // Edit 3 floats representing a color
            ImGui::Checkbox("Wireframe Mode", &use_wireframe_mode);
            ImGui::Checkbox("MSAA", &use_MSAA);
            ImGui::Checkbox("Outline", &draw_outline);
            ImGui::Checkbox("Bloom", &use_Bloom);

        }
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Model Setting")) {

            ImGui::SliderFloat3("rot_y", (float*)&rot_xyz, -180.0f, 180.0);
            ImGui::SliderFloat("scale_xyz", &scale_xyz, 0.05f, 3.0f);
            ImGui::SliderFloat("trans_x", &trans_x, -10.0f, 10.0f);
            
            ImGui::Spacing();
            ImGui::SliderFloat("model shininess", &shininess, 0, 1);
        }
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Light Setting")) {

            ImGui::DragFloat3("point light pos_xyz", (float*)&light_point_pos, 0.05f, -10.0f, 10.0f);
            ImGui::DragFloat("point light power", (float*)&light_point_power, 0.02f, 0.0f, 5.0f);
            ImGui::Spacing();

            ImGui::DragFloat3("direction light rot_xyz", (float*)&light_dir_rot, 5.0f, -180.0f, 180.0f);
            ImGui::DragFloat("direction light power", (float*)&light_dir_power, 0.02f, 0.0f, 5.0f);
            ImGui::Spacing();

            ImGui::DragFloat3("spot light pos_xyz", (float*)&light_spot_pos, 0.05f, -10.0f, 10.0f);
            ImGui::DragFloat3("spot light rot_xyz", (float*)&light_spot_rot, 5.0f, -180.0f, 180.0f);
            ImGui::DragFloat("spot light power", (float*)&light_spot_power, 0.02f, 0.0f, 5.0f);
        }
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Bloom Setting")) {
            ImGui::SliderFloat("bloom strength", &bloom_strength, 0.05f, 2.0f);
            ImGui::SliderInt("bloom blur", &bloom_blur, 1, 10);
        }
        ImGui::Spacing();

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

// process manual operations
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) mainCamera.Move(CAMERAMOVE::FORWARD,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) mainCamera.Move(CAMERAMOVE::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) mainCamera.Move(CAMERAMOVE::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) mainCamera.Move(CAMERAMOVE::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) mainCamera.Move(CAMERAMOVE::WDUP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) mainCamera.Move(CAMERAMOVE::WDDOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);

    mainCamera.Height = SCR_HEIGHT;
    mainCamera.Width = SCR_WIDTH;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = (float)xposIn;
    float ypos = (float)yposIn;

    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    if(!showMouse) mainCamera.Roate(xpos - lastMouseX, ypos - lastMouseY);
    lastMouseX = xpos;
    lastMouseY = ypos;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    mainCamera.ScaleFOV((float)yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        showMouse=!showMouse;
        glfwSetInputMode(window, GLFW_CURSOR, showMouse?GLFW_CURSOR_NORMAL:GLFW_CURSOR_DISABLED);
        firstMouse = true;
    }
        
}



int main(){

    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, false);// fix window size
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RTR", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, showMouse ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    // initialize GLAD *AFTER* creating a GLFWindow
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const char* version = (const char*)glGetString(GL_VERSION);
    printf("OpenGL version : %s\n", version);


    // ImGUI
    GUI::ImGUIInit(window);


    // models
    Model floor = Model(SHAPE::RECT);
    Model a_sphere = Model(SHAPE::SPHERE);
    Model main_model = Model();
    //main_model.LoadModel("Resource/dice/scene.gltf");
    main_model.LoadModel("Resource/hk_ump/scene.gltf");
    //main_model.LoadModel("Resource/higokumaru/scene.gltf");
    //main_model.LoadModel("Resource/pickaxe/scene.gltf");



    glm::mat4 floor_sp = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));
    floor_sp = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)) * floor_sp;
    floor_sp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -20.0f)) * floor_sp;
    floor.transform = floor_sp;


    // lights
    PointLight _light_point = PointLight();
    DirLight _light_dir = DirLight();
    SpotLight _light_spot = SpotLight();



    // shaders    
    Shader phong_shader = Shader("Shaders/BlinnPhong-NormalMap.vert", "Shaders/BlinnPhong-NormalMap.frag");
    //Shader phong_shader = Shader("Shaders/BlinnPhong.vert", "Shaders/BlinnPhong.frag");
    //Shader depth_shader = Shader("Shaders/DebugShader/depth.vert", "Shaders/DebugShader/depth.frag");
    Shader outline_shader = Shader("Shaders/Effect/outlining.vert", "Shaders/Effect/outlining.frag");
    //Shader reflect_shader = Shader("Shaders/Effect/reflect.vert", "Shaders/Effect/reflect.frag");
    //Shader refract_shader = Shader("Shaders/Effect/refract.vert", "Shaders/Effect/refract.frag");
    Shader pbr_shader = Shader("Shaders/PBR.vert", "Shaders/PBR.frag");


    // cameras
    //mainCamera.Move(CAMERAMOVE::WDUP, 1.0f);
    mainCamera.Move(CAMERAMOVE::BACKWARD, 8.0f);
    //mainCamera.Roate(0, 90.0f);
    //mainCamera.Far = 50.0f;
    mainCamera.type = CAMERATYPE::ORTHO;
    mainCamera.Height = 2.2 * mainCamera.Height;
    mainCamera.Width = 2.2 * mainCamera.Width;

    // skybox
    Skybox skybox = Skybox();



    // screen buffer
    unsigned int screenBuffer;
    glGenFramebuffers(1, &screenBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, screenBuffer);

    // screen texture
    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // bind screen texture to screen buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    // screen texture - blooming
    unsigned int screenBloomTexture;
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
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // bind the Renderbuffer Object to screen buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Screen Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // MS buffer/texture for MSAA
    unsigned int msBuffer;
    glGenFramebuffers(1, &msBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, msBuffer);

    unsigned int msTexture;
    glGenTextures(1, &msTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_samples, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msTexture, 0);

    unsigned int msBloomTexture;
    glGenTextures(1, &msBloomTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msBloomTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_samples, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, msBloomTexture, 0);

    //unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    unsigned int msRBO;
    glGenRenderbuffers(1, &msRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, msRBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER,msaa_samples, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: MS Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    // buffers for GaussianBlur on bloom texture
    GLuint pingpongFBO[2];
    GLuint pingpongBuffer[2];
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

    SimpleMesh rect = SimpleMesh(SHAPE::RECT);
    rect.RemoveTextures();
    Shader shaderBlur = Shader("Shaders/Postprocess/gaussianBlur.vert", "Shaders/Postprocess/gaussianBlur.frag");




    // screen mesh and shader
    SimpleMesh screen_mesh = SimpleMesh(SHAPE::RECT);
    screen_mesh.RemoveTextures();
    Shader screen_shader = Shader("Shaders/Postprocess/default.vert", "Shaders/Postprocess/default.frag");
    //Shader screen_shader = Shader("Shaders/Postprocess/kernal.vert", "Shaders/Postprocess/kernal.frag");


    // rendering loop
    while (!glfwWindowShouldClose(window)){

        // timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glm::mat4 model_sp = glm::scale(glm::mat4(1.0f), glm::vec3(scale_xyz, scale_xyz, scale_xyz));
        model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_xyz.z), glm::vec3(0.0, 0.0, 1.0)) * model_sp;
        model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_xyz.y), glm::vec3(0.0, 1.0, 0.0)) * model_sp;
        model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_xyz.x), glm::vec3(1.0, 0.0, 0.0)) * model_sp;
        model_sp = glm::translate(glm::mat4(1.0f), glm::vec3(trans_x, 0.0f, 0.0f))* model_sp;

        main_model.transform = model_sp;



        _light_point.Transform = glm::translate(glm::mat4(1.0f), light_point_pos);
        _light_point.Power = light_point_power;

        glm::mat4 light_dir_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_dir_rot[0]), glm::vec3(1.0, 0.0, 0.0));
        light_dir_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_dir_rot[1]), glm::vec3(0.0, 0.0, 1.0)) * light_dir_trans;
        light_dir_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_dir_rot[2]), glm::vec3(0.0, 1.0, 0.0)) * light_dir_trans;
        light_dir_trans[3] = _light_dir.Transform[3];
        _light_dir.Transform= light_dir_trans;
        _light_dir.Power = light_dir_power;


        glm::mat4 light_spot_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_spot_rot[0]), glm::vec3(1.0, 0.0, 0.0));
        light_spot_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_spot_rot[1]), glm::vec3(0.0, 0.0, 1.0)) * light_spot_trans;
        light_spot_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_spot_rot[2]), glm::vec3(0.0, 1.0, 0.0)) * light_spot_trans;
        light_spot_trans = glm::translate(glm::mat4(1.0f), light_spot_pos) * light_spot_trans;
        _light_spot.Transform = light_spot_trans;
        _light_spot.Power = light_spot_power;

        
        // first pass
        {
            if (use_MSAA) {
                glBindFramebuffer(GL_FRAMEBUFFER, msBuffer);
            }
            else {
                glBindFramebuffer(GL_FRAMEBUFFER, screenBuffer);
            }
            
            glDrawBuffers(2, attachments);

            glDisable(GL_FRAMEBUFFER_SRGB);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);
            glEnable(GL_CULL_FACE);

            glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            if (use_wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            phong_shader.use();

            phong_shader.setFloat("shininess", shininess);

            phong_shader.setFloat("point_light.power", _light_point.Power);
            phong_shader.setVec3("point_light.pos", glm::vec3(_light_point.Transform[3][0], _light_point.Transform[3][1], _light_point.Transform[3][2]));
            phong_shader.setVec3("point_light.ambient", _light_point.ambient);
            phong_shader.setVec3("point_light.diffuse", _light_point.diffuse);
            phong_shader.setVec3("point_light.specular", _light_point.specular);

            phong_shader.setFloat("dir_light.power", _light_dir.Power);
            phong_shader.setVec3("dir_light.dir", _light_dir.GetDirection());
            phong_shader.setVec3("dir_light.ambient", _light_dir.ambient);
            phong_shader.setVec3("dir_light.diffuse", _light_dir.diffuse);
            phong_shader.setVec3("dir_light.specular", _light_dir.specular);

            phong_shader.setFloat("spot_light.power", _light_spot.Power);
            phong_shader.setVec3("spot_light.pos", glm::vec3(_light_spot.Transform[3][0], _light_spot.Transform[3][1], _light_spot.Transform[3][2]));
            phong_shader.setVec3("spot_light.dir", _light_spot.GetDirection());
            phong_shader.setFloat("spot_light.cosR1", glm::cos(glm::radians(_light_spot.R1)));
            phong_shader.setFloat("spot_light.cosR2", glm::cos(glm::radians(_light_spot.R2)));
            phong_shader.setVec3("spot_light.ambient", _light_spot.ambient);
            phong_shader.setVec3("spot_light.diffuse", _light_spot.diffuse);
            phong_shader.setVec3("spot_light.specular", _light_spot.specular);

            phong_shader.setVec3("camera_pos", mainCamera.Position);
            phong_shader.setFloat("bloom_threshold", bloom_strength * bloom_strength);

            //glDisable(GL_FRAMEBUFFER_SRGB);
            //depth_shader.use();
            //depth_shader.setFloat("far", mainCamera.Far);
            //depth_shader.setFloat("near", mainCamera.Near);
            //floor.Draw(depth_shader, mainCamera);
            //dice.Draw(depth_shader, mainCamera);

            outline_shader.use();
            outline_shader.setFloat("line_width", 0.02f);
            outline_shader.setVec3("outline_color", glm::vec3(0.7f, 1.0f, 0.7f));

            //reflect_shader.use();
            //reflect_shader.setVec3("camera_pos", mainCamera.Position);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTextureID());

            //refract_shader.use();
            //refract_shader.setVec3("camera_pos", mainCamera.Position);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTextureID());
            
            pbr_shader.use();
            pbr_shader.setFloat("dir_light.power", _light_dir.Power);
            pbr_shader.setVec3("dir_light.dir", _light_dir.GetDirection());
            pbr_shader.setVec3("dir_light.color", _light_dir.diffuse);
            pbr_shader.setVec3("albedo", glm::vec3(0.0f,1.0f,0.2f));
            pbr_shader.setVec3("camera_pos", mainCamera.Position);
            pbr_shader.setVec3("camera_dir", mainCamera.Front);
            pbr_shader.setFloat("bloom_threshold", bloom_strength * bloom_strength);


            // meshes that may draw outline
            {
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);

                //main_model.Draw(phong_shader, mainCamera);
                //main_model.Draw(reflect_shader, mainCamera);
                //main_model.Draw(refract_shader, mainCamera);


                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 5; ++j) {
                        
                        glm::mat4 model_trans = glm::translate(glm::mat4(1.0f), glm::vec3(float(i*3 - 6.0f), float(j*3 - 6.0f), 0.0f));
                        a_sphere.transform = model_trans;

                        pbr_shader.setFloat("roughness", i * 0.25f);
                        pbr_shader.setFloat("metallic", j * 0.25f);

                        //a_sphere.Draw(phong_shader, mainCamera);
                        a_sphere.Draw(pbr_shader, mainCamera);

                    }
                }

                glStencilMask(0x00);
            }

            // meshes that never draw outline
            {

                //floor.Draw(phong_shader, mainCamera);

                //_light_point.Draw(mainCamera);
                //_light_dir.Draw(mainCamera);
                //_light_spot.Draw(mainCamera);
            }

            //skybox
            {
                glDepthFunc(GL_LEQUAL);
                //skybox.Draw(mainCamera);
                glDepthFunc(GL_LESS);
            }


            if(draw_outline){
                // draw outline
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                glDisable(GL_DEPTH_TEST);

                main_model.Draw(outline_shader, mainCamera);

                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF); // this will affect glClear
                glEnable(GL_DEPTH_TEST);
            }

        }
         
        // msaa
        if (use_MSAA) {
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
        {
            GLboolean first_iteration = true;
            GLuint iter = bloom_blur;
            shaderBlur.use();

            for (GLuint i = 0; i < iter; i++) {

                glBindFramebuffer(GL_READ_FRAMEBUFFER, pingpongFBO[0]);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingpongFBO[1]);

                glActiveTexture(0);
                glBindTexture(GL_TEXTURE_2D, first_iteration ? screenBloomTexture : pingpongBuffer[0]);
                shaderBlur.setBool("horizontal", true);
                rect.Draw(shaderBlur);

                glBindFramebuffer(GL_READ_FRAMEBUFFER, pingpongFBO[1]);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pingpongFBO[0]);

                glActiveTexture(0);
                glBindTexture(GL_TEXTURE_2D, pingpongBuffer[1]);
                shaderBlur.setBool("horizontal", false);
                rect.Draw(shaderBlur);


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

            screen_shader.use();
            screen_shader.setBool("useBloom", use_Bloom);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screenTexture);
            screen_shader.setInt("screenTexture", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffer[0]);
            screen_shader.setInt("screenBloomTexture", 1);
            screen_mesh.Draw(screen_shader);
        }


        // GUI
        {
            glDisable(GL_FRAMEBUFFER_SRGB);
            GUITick();
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // release resource and quit
    glfwTerminate();
    return 0;
}


