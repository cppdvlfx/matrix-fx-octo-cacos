#include <iostream>

extern "C"{
#include <glad/glad.h>
}
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    std::cout << "Demo Matrix Effect" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 600, "Demo Matrix Effect", NULL, NULL);
    if (!window){
        std::cout << "La ventana Demo Matrix Effect, no se pudo crear... " << std::endl;
        glfwTerminate();
        return -1; 
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "La libreria opengl no se pudo cargar" << std::endl; 
        glfwTerminate();
        return -1; 
    }


    
    while (!glfwWindowShouldClose(window)){
        processInput(window);


        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}