#include <iostream>

extern "C"{
#include <glad/glad.h>
}
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

FT_Library ft;
FT_Face face;
int openFont(FT_Face* faceptr, const std::string& fontFile);
int initFreeType(FT_Library* ftptr);
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

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (initFreeType(&ft)){
        glfwTerminate();
        return -1;
    }
    if (openFont(&face, "MatrixCodeNfi-YPPj.otf")){
        FT_Done_FreeType(ft);
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);


        glfwPollEvents();
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
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


int openFont(FT_Face* face, const std::string& fontFile){
    if (FT_New_Face(ft, fontFile.c_str(), 0, face)){
        std::cout << "No se pudo inicializar la fuente" << std::endl;
        return -1;
    }
    return 0;
}
int initFreeType(FT_Library* ftptr){
    auto initFreeTypeError = FT_Init_FreeType(&ft);
    if (initFreeTypeError){
        std::cout << "No se pudo inicializar LibFreetype2" << std::endl;
        return -1;
    }
    return 0;
}
