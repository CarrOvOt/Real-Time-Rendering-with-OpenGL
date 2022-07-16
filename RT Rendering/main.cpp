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
Camera mainCamera= Camera(SCR_HEIGHT, SCR_WIDTH);

// show mouse to use gui and hide mouse to use camera
bool showMouse = true;

// options & settings in GUI
bool use_wireframe_mode = false;

glm::vec3 background_color = glm::vec3(0.2f, 0.2f, 0.2f );

float rot_y = 0.0f;
float rot_z = 0.0f;
float scale_xyz = 1.0f;
float trans_x = 0.0f;
//glm::vec3 model_color = glm::vec3(0.2f, 0.5f, 0.5f);


int lighting_mode = 1;
glm::vec3 light_pos = glm::vec3(0.8f, 0.4f, 1.5f);



struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Light {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

Material model_material{ glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), 0.25f};
Light light_strength{ glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.5f,0.5f,0.5f) ,glm::vec3(1.0f,1.0f,1.0f) };

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
        }
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Model Setting")) {

            ImGui::SliderFloat("rot_y", &rot_y, -180.0f, 180.0);
            ImGui::SliderFloat("rot_z", &rot_z, -180.0f, 180.0);
            ImGui::SliderFloat("scale_xyz", &scale_xyz, 0.1f, 5.0f);
            ImGui::SliderFloat("trans_x", &trans_x, -10.0f, 10.0f);
            //ImGui::ColorEdit3("model color", (float*)&model_color);
            
            ImGui::Spacing();
            ImGui::ColorEdit3("model ambient", (float*)&model_material.ambient);
            ImGui::ColorEdit3("model diffuse", (float*)&model_material.diffuse);
            ImGui::ColorEdit3("model specular", (float*)&model_material.specular);
            ImGui::SliderFloat("model shininess", &model_material.shininess, 0, 1);
        }
        ImGui::Spacing();

        ImGui::RadioButton("Ambient", &lighting_mode, 1<<1); ImGui::SameLine();
        ImGui::RadioButton("Diffuse", &lighting_mode, 1<<2); ImGui::SameLine();
        ImGui::RadioButton("Specular", &lighting_mode, 1<<3); ImGui::SameLine();
        ImGui::RadioButton("ALL", &lighting_mode, 1); 
        ImGui::Spacing();

        ImGui::DragFloat3("light pos_xyz", (float*)&light_pos, 0.05f, -10.0f, -10.0f);
        ImGui::ColorEdit3("light ambient", (float*)&light_strength.ambient);
        ImGui::ColorEdit3("light diffuse", (float*)&light_strength.diffuse);
        ImGui::ColorEdit3("light specular", (float*)&light_strength.specular);

        ImGui::Spacing();

        

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
    //glfwWindowHint(GLFW_RESIZABLE, false);// fix window size
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

    // ImGUI
    GUI::ImGUIInit(window);

   
    // mesh & shader & camera
    Model _model = Model();
    Model _light = Model(SHAPE::CUBE, glm::vec3(1.0f, 1.0f, 1.0f));
    Shader phong_shader = Shader("Shaders/Phong.vert", "Shaders/Phong.frag");



    // GL settings
    glEnable(GL_DEPTH_TEST);

    // rendering loop
    while (!glfwWindowShouldClose(window)){

        // timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glm::mat4 model_sp = glm::scale(glm::mat4(1.0f), glm::vec3(scale_xyz, scale_xyz, scale_xyz));
        model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_z), glm::vec3(0.0, 0.0, 1.0)) * model_sp;
        model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_y), glm::vec3(0.0, 1.0, 0.0)) * model_sp;
        model_sp = glm::translate(glm::mat4(1.0f), glm::vec3(trans_x, 0.0f, 0.0f))* model_sp;
        _model.tranform = model_sp;

        _light.tranform= glm::translate(glm::mat4(1.0f), light_pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
        SimpleMesh* m = (SimpleMesh*)_light.meshes[0];
        m->SetColor(light_strength.diffuse);
        
        glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        if (use_wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        phong_shader.use();
        phong_shader.setInt("lighting_mode", lighting_mode);


        phong_shader.setVec3("material.ambient", model_material.ambient);
        phong_shader.setVec3("material.diffuse", model_material.diffuse);
        phong_shader.setVec3("material.specular", model_material.specular);
        phong_shader.setFloat("material.shininess", model_material.shininess);

        phong_shader.setVec3("light.pos", light_pos);
        phong_shader.setVec3("light.ambient", light_strength.ambient);
        phong_shader.setVec3("light.diffuse", light_strength.diffuse);
        phong_shader.setVec3("light.specular", light_strength.specular);


        phong_shader.setVec3("camera_pos", mainCamera.Position);

        //_model.Draw(mainCamera);
        _model.Draw(phong_shader,mainCamera);
        _light.Draw(mainCamera);


        GUITick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // release resource and quit
    glfwTerminate();
    return 0;
}


