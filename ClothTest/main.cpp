#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "Camera.h"

#include "Ghost.h"

int main()
{
    //create window
    glfwInit();
    glm::uvec2 windowSize(1200, 900);
    GLFWwindow* window = glfwCreateWindow(windowSize.x, windowSize.y, "Testing cloth", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    
    //load OpenGL using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw;
    }
    
    //set up input interface
    Input input(window);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    Camera camera(90.f, windowSize, 0.1f, 100.f, { 20, 8, -15.0f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });

    Ghost ghost(input, glm::vec3(12.f, 5.f, 5.2f));

    //update window/simulation
    auto lastUpdate = std::chrono::high_resolution_clock::now();
    float timer = 0;
    while (!glfwWindowShouldClose(window))
    {
        //poll close event
        glfwPollEvents();

        //gets the elapsed amount of seconds since the start of the last update
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> timeDiff = now - lastUpdate;
        lastUpdate = now;

        //update cloth simulation
        float elapsed = timeDiff.count();
        timer += elapsed;
        ghost.update(elapsed);

        //draw cloth simulation onto window
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ghost.draw(camera);
        glfwSwapBuffers(window);

    }

    //terminate glfw
    glfwTerminate();

    return 0;
}
