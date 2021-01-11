#include <map>
#include <cctype>
#include <random>
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
void renderText(Shader& shader, const std::string& messageToRender, float cx, float cy, float scale, glm::vec3 color); 
struct glyphData{
    unsigned int textureID;
    glm::ivec2 bearing;
    glm::ivec2 size;
    unsigned int advance;
};
std::map<char, glyphData> c_glyphdata{};
int maxheight{0};

glm::ivec2 screensize{1600, 900};
glm::vec2 screensizef{1600.0f, 900.0f};

unsigned int VAO, VBO;
void createRenderingDataContainer();

float calcstringtrackwidth(const std::string&message, float scale);

int main() {
    std::cout << "Demo Matrix Effect" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(screensize.x, screensize.y, "Demo Matrix Effect", NULL, NULL);
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
    auto canvasprojection = glm::ortho(0.0f, screensizef.x, 0.0f, screensizef.y, -1.0f, 1.0f);
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
    if (createGlyphTextures(&face, 24)){
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        glfwTerminate();
        return -1;

    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    createRenderingDataContainer();

    auto messagetorender = "THE CPP DEVIL -- 2021 -- the cpp devil -- 2021";
    auto stringwidth = calcstringtrackwidth(messagetorender, 1.0);

    std::random_device rd;
    std::uniform_int_distribution<int> uniformDistribution(0, screensize.x - 1);
    auto randomColumn = 1.0f * uniformDistribution(rd);

    auto framesPerSecond = 60.0f;
    auto trackSpeed = -1.0f * maxheight / framesPerSecond;//Character height per frame
    auto periodPerFrame = 1.0f / framesPerSecond;
    auto tNow = glfwGetTime();
    auto tNextFrame = tNow + periodPerFrame;
    auto yCoordinate = screensizef.y * 1.0f;

    while (!glfwWindowShouldClose(window)){

        while (tNow >= tNextFrame){
            tNextFrame += periodPerFrame;
            yCoordinate += trackSpeed;
        }
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderText(shader, messagetorender, randomColumn - stringwidth, yCoordinate, 1.0f, glm::vec3(0.3f, 0.3f, 0.9f));
        glfwSwapBuffers(window);
        glfwPollEvents();

        tNow = glfwGetTime();
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

        auto glyphchar = static_cast<char>(index);
        std::cout << "Glyph [" << (std::isprint(glyphchar) ? glyphchar : ' ') << "][" << static_cast<unsigned int> (glyphchar) <<"]:" << std::endl;
        auto glyph_bearing = glm::ivec2(faceptr[0]->glyph->bitmap_left, faceptr[0]->glyph->bitmap_top);
        auto glyph____size = glm::ivec2(faceptr[0]->glyph->bitmap.width, faceptr[0]->glyph->bitmap.rows);
        auto ______advance = static_cast<unsigned int>(faceptr[0]->glyph->advance.x);
        std::cout << "Bearing: " << glyph_bearing.x << ", " << glyph_bearing.y << std::endl;
        std::cout << "Size   : " << glyph____size.x << ", " << glyph____size.y << std::endl;
        std::cout << "Advance: " << ______advance << ", hex: " << std::hex << ______advance << std::dec << std::endl;

        if (glyph____size.y > maxheight) maxheight = glyph____size.y;
        auto glyph = glyphData{
            texture,
            glyph_bearing,
            glyph____size,
            ______advance
        };
        c_glyphdata[glyphchar] = glyph;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return 0;
}
void renderText(Shader& shader, const std::string& messageToRender, float cx, float cy, float scale, glm::vec3 color){

    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    for (auto const& c : messageToRender){
        auto glyph = c_glyphdata[c];
        auto pos__ = glm::vec2(cx + glyph.bearing.x * scale, cy - (glyph.size.y - glyph.bearing.y) * scale);
        auto size_ = glm::vec2(glyph.size.x , glyph.size.y) * scale;
        float vertices [6][4] = {
            {pos__.x,           pos__.y + size_.y,  0.0f, 0.0f},
            {pos__.x,           pos__.y,            0.0f, 1.0f},
            {pos__.x + size_.x, pos__.y,            1.0f, 1.0f},

            {pos__.x,           pos__.y + size_.y,  0.0f, 0.0f},
            {pos__.x + size_.x, pos__.y,            1.0f, 1.0f},
            {pos__.x + size_.x, pos__.y + size_.y,  1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, glyph.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        cy += (glyph.size.y + (glyph.advance >> 6) - glyph.size.x) * scale;

    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}
void createRenderingDataContainer(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
float calcstringtrackwidth(const std::string& messageToRender, float scale){
    float cx = 0.0f;
    float max = 0.0f;
    for (auto const& c : messageToRender){
        auto glyph = c_glyphdata[c];
        auto adv = (glyph.advance >> 6) * scale;
        if (adv > max){
            max = adv;
        }
    }
    return 0.5f * max;
}