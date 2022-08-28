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
#include "scene.h"
#include "render.h"
#include "gui.h"


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


    ForwardRender* mainRender = new ForwardRender();
    mainRender->ForwardRenderSet();



    Scene* mainScene = new Scene();
    mainScene->Models.push_back(Model("Resource/hk_ump/scene.gltf"));

    mainScene->mainShader = Shader("Shaders/PBR_Texture.vert", "Shaders/PBR_Texture.frag");
    mainScene->outlineShader = Shader("Shaders/Effect/outlining.vert", "Shaders/Effect/outlining.frag");

    mainScene->SpotLights.push_back(SpotLight());
    mainScene->DirLights.push_back(DirLight());
    mainScene->PointLights.push_back(PointLight());

    mainScene->mainCamera = Camera(SCR_HEIGHT / 200.0f, SCR_WIDTH / 200.0f);
    mainScene->mainCamera.Move(CAMERAMOVE::BACKWARD, 8.0f);

    mainScene->mainSkybox = Skybox();
    mainScene->setIBL();



    // rendering loop
    while (!glfwWindowShouldClose(window)){

        // timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);


        // process gui input
        {
            // model and lights settings
            glm::mat4 model_sp = glm::scale(glm::mat4(1.0f), glm::vec3(scale_xyz, scale_xyz, scale_xyz));
            model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_xyz.z), glm::vec3(0.0, 0.0, 1.0)) * model_sp;
            model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_xyz.y), glm::vec3(0.0, 1.0, 0.0)) * model_sp;
            model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_xyz.x), glm::vec3(1.0, 0.0, 0.0)) * model_sp;
            model_sp = glm::translate(glm::mat4(1.0f), glm::vec3(trans_x, 0.0f, 0.0f)) * model_sp;
            mainScene->Models[0].transform = model_sp;

            mainScene->PointLights[0].Transform = glm::translate(glm::mat4(1.0f), light_point_pos);
            mainScene->PointLights[0].Power = light_point_power;

            glm::mat4 light_dir_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_dir_rot[0]), glm::vec3(1.0, 0.0, 0.0));
            light_dir_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_dir_rot[1]), glm::vec3(0.0, 0.0, 1.0)) * light_dir_trans;
            light_dir_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_dir_rot[2]), glm::vec3(0.0, 1.0, 0.0)) * light_dir_trans;
            light_dir_trans[3] = mainScene->DirLights[0].Transform[3];
            mainScene->DirLights[0].Transform = light_dir_trans;
            mainScene->DirLights[0].Power = light_dir_power;

            glm::mat4 light_spot_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_spot_rot[0]), glm::vec3(1.0, 0.0, 0.0));
            light_spot_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_spot_rot[1]), glm::vec3(0.0, 0.0, 1.0)) * light_spot_trans;
            light_spot_trans = glm::rotate(glm::mat4(1.0f), glm::radians(light_spot_rot[2]), glm::vec3(0.0, 1.0, 0.0)) * light_spot_trans;
            light_spot_trans = glm::translate(glm::mat4(1.0f), light_spot_pos) * light_spot_trans;
            mainScene->SpotLights[0].Transform = light_spot_trans;
            mainScene->SpotLights[0].Power = light_spot_power;

            // camera
            mainScene->mainCamera = mainCamera;

            // render settings
            mainRender->draw_outline = draw_outline;
            mainRender->use_MSAA = use_MSAA;
            mainRender->use_Bloom = use_Bloom;
            mainRender->bloom_blur_iter = bloom_blur;
            

        }

        // Draw scene
        mainRender->ForwardRenderDraw(mainScene);

        // Draw GUI
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


