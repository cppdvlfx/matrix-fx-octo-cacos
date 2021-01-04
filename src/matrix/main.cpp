#include <iostream>

extern "C"{
#include <glad/glad.h>
}
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

FT_Library ft;
FT_Face face;
int openFont(FT_Face* faceptr, const std::string& fontFile);
int initFreeType(FT_Library* ftptr);
int createGlyphTextures(FT_Face* faceptr, unsigned long pixelHeight);
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

    Shader shader("text.vs", "text.fs");
    auto canvasprojection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(canvasprojection));

    
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
int createGlyphTextures(FT_Face* faceptr, unsigned long pixelHeight){

    FT_Set_Pixel_Sizes(*faceptr, 0, pixelHeight);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (auto index = 0u; index < 128; ++index){
        if (FT_Load_Char(*faceptr, index, FT_LOAD_RENDER)){
            std::cout << "ERROR Creating texture for character: " << static_cast<char>(index) << std::endl;
            return -1;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            (*faceptr)->glyph->bitmap.width,
            (*faceptr)->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
            );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return 0;
}

