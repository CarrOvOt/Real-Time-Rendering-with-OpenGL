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
bool showMouse = false;

// options & settings in GUI
bool use_wireframe_mode = false;

float rot_y = 0.0f;
float rot_z = 0.0f;
float scale_xyz = 1.0f;
float trans_x = 0.0f;


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

        ImGui::Begin("Hello, ImGUI!");              // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Some settings");               // Display some text (you can use a format strings too)

        ImGui::Checkbox("Wireframe Mode", &use_wireframe_mode);

        ImGui::SliderFloat("rot_y", &rot_y, -180.0f, 180.0);
        ImGui::SliderFloat("rot_z", &rot_z, -180.0f, 180.0);
        ImGui::SliderFloat("scale_xyz", &scale_xyz, 0.1f, 5.0f);
        ImGui::SliderFloat("trans_x", &trans_x, -10.0f, 10.0f);

        ////ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //    counter++;
        //ImGui::SameLine();
        //ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize GLAD *AFTER* creating a GLFWindow
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ImGUI
    GUI::ImGUIInit(window);

   
    // mesh & shader & camera
    Model _model = Model();
    Shader _shader = Shader("Shaders/vshader.vs", "Shaders/fshader.fs");


    // coordinate transformation
    glm::mat4 model_sp_original = glm::mat4(1.0f);
    glm::mat4 view_sp_original = glm::mat4(1.0f);
    glm::mat4 proj_sp_original = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


    // GL settings
    glEnable(GL_DEPTH_TEST);

    // rendering loop
    while (!glfwWindowShouldClose(window)){

        // timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glm::mat4 model_sp = glm::scale(glm::mat4(1.0f), glm::vec3(scale_xyz, scale_xyz, scale_xyz)) * model_sp_original;
        model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_z), glm::vec3(0.0, 0.0, 1.0)) * model_sp;
        model_sp = glm::rotate(glm::mat4(1.0f), glm::radians(rot_y), glm::vec3(0.0, 1.0, 0.0)) * model_sp;
        model_sp = glm::translate(glm::mat4(1.0f), glm::vec3(trans_x, 0.0f, 0.0f))* model_sp;

        glm::mat4 view_sp = glm::translate(view_sp_original, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 proj_sp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        

        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


        if (use_wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //draw our mesh
        _shader.use();
        _shader.setMat4("model_sp", model_sp);
        _shader.setMat4("view_sp", mainCamera.GetViewMatrix());
        _shader.setMat4("proj_sp", mainCamera.GetProjMatrix());

        _model.Draw(_shader);


        GUITick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // release resource and quit
    glfwTerminate();
    return 0;
}


