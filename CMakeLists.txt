cmake_minimum_required(VERSION 3.17)
project(matrix_fx)

include(FetchContent)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

# -- 3rd party dependencies --
include(cmake/ExternalProjects.cmake)


add_executable(matrix_fx src/matrix/main.cpp src/glad/glad.c)
add_dependencies(matrix_fx glfw glm learnopengl)

target_include_directories(matrix_fx PRIVATE
        include

        ${glm_INCLUDE_DIR}
        ${glfw_INCLUDE_DIR}
        ${freetype2_INCLUDE_DIR}
        ${learnopengl_INCLUDE_DIR})


target_link_libraries(matrix_fx PRIVATE
        ${glfw_LIBRARY_DIR}/libglfw3.a


        #system
        -lassimp
        -lharfbuzz
        -lfreetype
        -lbrotlidec-static
        -lbrotlicommon-static
        -lpng
        -lbz2
        -lz

        )


add_custom_command(
        TARGET matrix_fx
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/matrix/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
add_custom_command(
        TARGET matrix_fx
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${learnopengl_REPOCNT_DIR}/src/7.in_practice/2.text_rendering/text.vs ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_command(
        TARGET matrix_fx
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${learnopengl_REPOCNT_DIR}/src/7.in_practice/2.text_rendering/text.fs ${CMAKE_CURRENT_BINARY_DIR}
)
