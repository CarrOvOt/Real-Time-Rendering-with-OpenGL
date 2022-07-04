#include <glad/glad.h>
#include <glfw3.h>

#include "shader.h"
#include "simpleMesh.h"

#include <iostream>

int main(){

    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "RTR", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // initialize GLAD *AFTER* creating a GLFWindow
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }




   
    SimpleMesh _mesh = SimpleMesh(SHAPE::CUBE);
    Shader _shader = Shader("Shaders/vshader.vs", "Shaders/fshader.fs");



    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // Wireframe Mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // Default Mode

    // rendering loop
    while (!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draw our mesh
        _shader.use();
        _mesh.Draw();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // release resource and quit
    glfwTerminate();
    return 0;
}