#include "viewer.h"
#include <GLFW/glfw3.h>
#include <stdio.h>


int run_viewer(DTM* dtm)
{
    if (dtm == NULL || dtm->triangle_count == 0) {
        printf("Error: No triangulation data available for viewer\n");
        return 0;
    }

    if (!glfwInit()) {
        printf("Error: Failed to initialize GLFW\n");
        return 0;
    }
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "DTM 3D Viewer", NULL, NULL);
    if (!window) {
        printf("Error: Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);

    glEnable(GL_DEPTH_TEST);

    printf("Graphic window opened successfule\n");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 1;
}

