#include "../include/glad/glad.h"
#include <iostream>
#include <GLFW/glfw3.h>

#include "../include/the_cube/stb_image.h"
#include "../include/the_cube/shader.h"
#include "../include/the_cube/camera.h"
#include "../include/the_cube/cube.h"
#include "../include/the_cube/tile.h"
#include "../include/the_cube/constants.h"

void framebuffer_size_callback(GLFWwindow* window, int width =  Screen::width, 
                                                   int height = Screen::height);
void processInput(GLFWwindow *window, the_cube::Cube& cube);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(Screen::width, Screen::height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    
    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    Shader tile_shader("shaders/tile_shader.vs", "shaders/tile_shader.fs");

    the_cube::Cube cube(glm::vec3(.0f), 1.f);
    Camera c(glm::vec3(1.5f,3.f,5), 45.f);
/*    the_cube::Tile tile(std::array<glm::vec3, 6>{
                        glm::vec3(0.25f,0.f,-0.25f),                         
                        glm::vec3(-0.25f,0.f,-0.25f),                         
                        glm::vec3(0.25f,0.f,0.25f),                         

                        glm::vec3(-0.25f,0.f,-0.25f),                         
                        glm::vec3(-0.25f,0.f,0.25f),                         
                        glm::vec3(0.25f,0.f,0.25f)}
                        );*/
    the_cube::Tile tile(glm::vec3(0.f,0.f,0.f), glm::vec3(0.00f,1.0f,0.0f));

    shader.use();
    shader.setMat4("projection", c.get_projection_matrix());
    tile_shader.use();
    tile_shader.setMat4("projection", c.get_projection_matrix());


    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //cube.rotate(30,the_cube::RotationDirection::LEFT);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, cube);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("model", cube.get_model_matrix());
        shader.setMat4("view", c.get_view_matrix());
        cube.render();
       
        tile_shader.use();
        tile_shader.setVec3("tile_color", tile.get_color());
        tile_shader.setMat4("view", c.get_view_matrix());
        tile.render();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
//    cube.delete_buffers();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, the_cube::Cube& cube)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cube.roll(1, the_cube::RotationDirection::RIGHT);
    }
    else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cube.roll(1, the_cube::RotationDirection::LEFT);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cube.roll(1, the_cube::RotationDirection::FORWARD);
    }
    else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cube.roll(1, the_cube::RotationDirection::BACKWARD);
    }
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
