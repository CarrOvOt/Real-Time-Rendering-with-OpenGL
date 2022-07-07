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
#include "simpleMesh.h"
#include "gui.h"

// others
#include <iostream>


// settings
static unsigned int SCR_WIDTH = 1280;
static unsigned int SCR_HEIGHT = 720;


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


void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}



int main(){

    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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


    // initialize GLAD *AFTER* creating a GLFWindow
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ImGUI
    GUI::ImGUIInit(window);


   
    // mesh & shader
    SimpleMesh _mesh = SimpleMesh(SHAPE::CUBE);
    Shader _shader = Shader("Shaders/vshader.vs", "Shaders/fshader.fs");


    // coordinate transformation
    glm::mat4 model_sp_original = glm::mat4(1.0f);
    glm::mat4 view_sp_original = glm::mat4(1.0f);
    glm::mat4 proj_sp_original = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


    // GL settings
    glEnable(GL_DEPTH_TEST);

    // rendering loop
    while (!glfwWindowShouldClose(window)){

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
        _shader.setMat4("view_sp", view_sp);
        _shader.setMat4("proj_sp", proj_sp);

        _mesh.Draw();


        GUITick();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // release resource and quit
    glfwTerminate();
    return 0;
}


