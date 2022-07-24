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

glm::vec3 background_color = glm::vec3(0.0f, 0.0f, 0.0f );

float rot_y = 0.0f;
float rot_z = 0.0f;
float scale_xyz = 0.5f;
float trans_x = 0.0f;


glm::vec3 light_point_pos = glm::vec3(0.0f, 0.65f, -3.0f);
float light_point_power=1.0f;

glm::vec3 light_dir_rot = glm::vec3(0.0f, 0.0f, 0.0f);
float light_dir_power = 1.0f;

glm::vec3 light_spot_pos = glm::vec3(0.1f, 0.1f, 0.0f);
glm::vec3 light_spot_rot = glm::vec3(-45.0f, 0.0f, 0.0f);
float light_spot_power = 1.0f;


struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};


Material model_material{ glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f), glm::vec3(1.0f,1.0f,1.0f), 0.25f};

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
            ImGui::SliderFloat("scale_xyz", &scale_xyz, 0.05f, 3.0f);
            ImGui::SliderFloat("trans_x", &trans_x, -10.0f, 10.0f);
            
            ImGui::Spacing();
            ImGui::SliderFloat("model shininess", &model_material.shininess, 0, 1);
        }
        ImGui::Spacing();

        ImGui::DragFloat3("point light pos_xyz", (float*)&light_point_pos, 0.05f, -10.0f, 10.0f);
        ImGui::DragFloat("point light power", (float*)&light_point_power, 0.02f, 0.0f, 5.0f);
        ImGui::Spacing();

        ImGui::DragFloat3("direction light rot_xyz", (float*)&light_dir_rot, 5.0f, -180.0f, 180.0f);
        ImGui::DragFloat("direction light power", (float*)&light_dir_power, 0.02f, 0.0f, 5.0f);
        ImGui::Spacing();

        ImGui::DragFloat3("spot light pos_xyz", (float*)&light_spot_pos, 0.05f, -10.0f, 10.0f);
        ImGui::DragFloat3("spot light rot_xyz", (float*)&light_spot_rot, 5.0f, -180.0f, 180.0f);
        ImGui::DragFloat("spot light power", (float*)&light_spot_power, 0.02f, 0.0f, 5.0f);
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
    Model _model = Model(SHAPE::CUBE);
    Model floor = Model(SHAPE::RECT);
    Model dice = Model();
    dice.LoadModel("C:/Users/96904/Desktop/free_hq__pbr_game_model_metallic_sapphire_dice/scene.gltf");

    PointLight _light_point = PointLight();
    DirLight _light_dir = DirLight();
    SpotLight _light_spot = SpotLight();

    glm::mat4 floor_sp = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));
    floor_sp = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)) * floor_sp;
    floor_sp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -20.0f)) * floor_sp;
    floor.transform = floor_sp;


    Shader phong_shader = Shader("Shaders/BlinnPhong.vert", "Shaders/BlinnPhong.frag");
    Shader depth_shader = Shader("Shaders/DebugShader/depth.vert", "Shaders/DebugShader/depth.frag");
    //Shader phong_shader = Shader("Shaders/Phong.vert", "Shaders/Phong.frag");


    mainCamera.Move(CAMERAMOVE::WDUP, 1.0f);
    mainCamera.Move(CAMERAMOVE::BACKWARD, 2.0f);
    mainCamera.Roate(0, 100.0f);
    mainCamera.Far = 50.0f;
    //mainCamera.type = CAMERATYPE::ORTHO;


    // GL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

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
        _model.transform = model_sp;

        dice.transform = model_sp;



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

        
        glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        if (use_wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        phong_shader.use();

        phong_shader.setFloat("material.shininess", model_material.shininess);

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

        glDisable(GL_FRAMEBUFFER_SRGB);
        depth_shader.use();
        depth_shader.setFloat("far", mainCamera.Far);
        depth_shader.setFloat("near", mainCamera.Near);
        //_model.Draw(depth_shader,mainCamera);
        floor.Draw(depth_shader, mainCamera);
        dice.Draw(depth_shader, mainCamera);

        ////_model.Draw(phong_shader,mainCamera);
        //floor.Draw(phong_shader, mainCamera);
        //dice.Draw(phong_shader, mainCamera);

        //_light_point.Draw(mainCamera);
        //_light_dir.Draw(mainCamera);
        //_light_spot.Draw(mainCamera);
         


        GUITick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // release resource and quit
    glfwTerminate();
    return 0;
}


