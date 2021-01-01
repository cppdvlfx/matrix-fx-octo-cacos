cmake_minimum_required(VERSION 3.17)
project(matrix_fx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

message (STATUS "-----------------------------------------------------------------------")
message (STATUS "NLOHMANN JSON ---------------------------------------------------------")

include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.9.1
)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

message (STATUS "-----------------------------------------------------------------------")
message (STATUS "GLM -------------------------------------------------------------------")

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
)
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
    include_directories(${glm_SOURCE_DIR})
    set(GLM_DIR ${glm_SOURCE_DIR}/cmake/glm)
    message(STATUS "added glm source: ${glm_SOURCE_DIR}")
    message(STATUS "added glm binary: ${glm_BINARY_DIR}")
    message(STATUS "GLM_DIR ${glm_SOURCE_DIR}/cmake/glm")
endif()


message (STATUS "-----------------------------------------------------------------------")
message (STATUS "GLAD ------------------------------------------------------------------")

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v0.1.34
)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${glad_SOURCE_DIR})
    message(STATUS "added glad source: ${glad_SOURCE_DIR}")
    message(STATUS "added glad binary: ${glad_BINARY_DIR}")
endif()


message (STATUS "-----------------------------------------------------------------------")
message (STATUS "ASSIMP ----------------------------------------------------------------")

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.0.1

)
FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)
    FetchContent_Populate(assimp)
    add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${assimp_SOURCE_DIR})
    set(ASSIMP_DIR ${assimp_BINARY_DIR})
    message(STATUS "added assimp source: ${assimp_SOURCE_DIR}")
    message(STATUS "added assimp binary: ${assimp_BINARY_DIR}")
    message(STATUS "ASSIMP_DIR: ${assimp_BINARY_DIR}")

endif()

message (STATUS "-----------------------------------------------------------------------")
message (STATUS "GLFW ------------------------------------------------------------------")
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.2
)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${glfw_SOURCE_DIR}/include)
    set(GLFW3_DIR ${glfw_BINARY_DIR}/src)
    message(STATUS "added glfw source: ${glfw_SOURCE_DIR}")
    message(STATUS "added glfw binary: ${glfw_BINARY_DIR}")
    message(STATUS "GLFW3_DIR: ${GLFW3_DIR}")

endif()

message (STATUS "-----------------------------------------------------------------------")
message (STATUS "HARFBUZZ ------------------------------------------------------------------")
FetchContent_Declare(
        harfbuzz
        GIT_REPOSITORY https://github.com/harfbuzz/harfbuzz.git
        GIT_TAG 2.7.4
)
FetchContent_GetProperties(harfbuzz)
if(NOT harfbuzz_POPULATED)
    FetchContent_Populate(harfbuzz)
    add_subdirectory(${harfbuzz_SOURCE_DIR} ${harfbuzz_BINARY_DIR} EXCLUDE_FROM_ALL)
    message(STATUS "added glfw source: ${glfw_SOURCE_DIR}")
    message(STATUS "added glfw binary: ${glfw_BINARY_DIR}")
    message(STATUS "GLFW3_DIR: ${GLFW3_DIR}")

endif()


message (STATUS "-----------------------------------------------------------------------")
message (STATUS "FREETYPE2 -------------------------------------------------------------")

FetchContent_Declare(
        freetype2
        GIT_REPOSITORY https://github.com/aseprite/freetype2.git
        GIT_TAG VER-2-10-0
)
FetchContent_GetProperties(freetype2)
if(NOT freetype2_POPULATED)
    FetchContent_Populate(freetype2)
    add_subdirectory(${freetype2_SOURCE_DIR} ${freetype2_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${freetype2_SOURCE_DIR}/include)
    message(STATUS "added freetype2 source: ${freetype2_SOURCE_DIR}")
    message(STATUS "added freetype2 binary: ${freetype2_BINARY_DIR}")
endif()


message (STATUS "-----------------------------------------------------------------------")
message (STATUS "LEARNOPENGL -----------------------------------------------------------")

FetchContent_Declare(
        learnopengl
        GIT_REPOSITORY https://github.com/JoeyDeVries/LearnOpenGL.git
        GIT_TAG master
)
FetchContent_GetProperties(learnopengl)
if(NOT learnopengl_POPULATED)
    FetchContent_Populate(learnopengl)

    #this hack was needed in MINGW platform
    file(COPY ${glfw_BINARY_DIR}/CMakeFiles/Export/lib/cmake/glfw3/glfw3Targets.cmake DESTINATION ${glfw_BINARY_DIR}/src/)

    message(STATUS "${CMAKE_CURRENT_LIST_DIR}/glfw3Targets.cmake")
    add_subdirectory(${learnopengl_SOURCE_DIR} ${learnopengl_BINARY_DIR} EXCLUDE_FROM_ALL)
    include_directories(${learnopengl_SOURCE_DIR}/includes)

    message(STATUS "added learnopengl source: ${learnopengl_SOURCE_DIR}")
    message(STATUS "added learnopengl binary: ${learnopengl_BINARY_DIR}")
endif()

message(STATUS "All dependencies are in place now.")
message (STATUS "-----------------------------------------------------------------------")


add_executable(matrix_fx matrix/src/main.cpp)