#include "viewer.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>


static float rotation_x = 45.0f;
static float rotation_z = 45.0f;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_UP:       rotation_x += 5.0f; break;
            case GLFW_KEY_DOWN:     rotation_x -= 5.0f; break;
            case GLFW_KEY_LEFT:     rotation_z -= 5.0f; break;
            case GLFW_KEY_RIGHT:    rotation_z += 5.0f; break;
            case GLFW_KEY_ESCAPE:   glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        }
    }
}


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
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);

    float min_x = dtm->points[0].x, max_x = dtm->points[0].x;
    float min_y = dtm->points[0].y, max_y = dtm->points[0].y;
    float min_z = dtm->points[0].z, max_z = dtm->points[0].z;

    for (unsigned int i = 1; i < dtm->point_count; i++) {
        if (dtm->points[i].x < min_x) min_x = dtm->points[i].x;
        if (dtm->points[i].y < min_y) min_y = dtm->points[i].y;
        if (dtm->points[i].z < min_z) min_z = dtm->points[i].z;
        if (dtm->points[i].x > max_x) max_x = dtm->points[i].x;
        if (dtm->points[i].y > max_y) max_y = dtm->points[i].y;
        if (dtm->points[i].z > max_z) max_z = dtm->points[i].z;

    }

    float dx = max_x - min_x;
    float dy = max_y - min_y;
    float dz = max_z - min_z;
    float max_dim = (dx > dy) ? dx : dy;
    max_dim = (max_dim > dz) ? max_dim : dz;
    float scale = (max_dim > 0.0001f) ? (1.2f / max_dim) : 1.0f;
    
    float z_range = (max_z - min_z > 0.0001f) ? (max_z - min_z) : 1.0f;

    float center_x = (min_x + max_x) * 0.5f;
    float center_y = (min_y + max_y) * 0.5f;
    float center_z = (min_z + max_z) * 0.5f;

    printf("Rendering 3D scene");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glLoadIdentity();

        glScalef(scale, scale, scale);
        glRotatef(rotation_x, 1.0f, 0.0f, 0.0f);
        glRotatef(rotation_z, 0.0f, 0.0f, 1.0f);
        glTranslatef(-center_x, -center_y, -center_z);

        glBegin(GL_TRIANGLES);
        
        for (unsigned int i = 0; i < dtm->triangle_count; i++) {
            Triangle t = dtm->triangles[i];

            Point p1 = dtm->points[t.p1];
            float factor1 = (p1.z - min_z) / z_range;
            glColor3f(0.1f, 0.3f + factor1 * 0.7f, 0.1f);
            glVertex3f(p1.x, p1.y, p1.z);

            Point p2 = dtm->points[t.p2];
            float factor2 = (p2.z - min_z) / z_range;
            glColor3f(0.1f, 0.3f + factor2 * 0.7f, 0.1f);
            glVertex3f(p2.x, p2.y, p2.z);

            Point p3 = dtm->points[t.p3];
            float factor3 = (p3.z - min_z) / z_range;
            glColor3f(0.1f, 0.3f + factor3 * 0.7f, 0.1f);
            glVertex3f(p3.x, p3.y, p3.z);
        }
        glEnd();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f);
        glBegin(GL_TRIANGLES);
        glColor3f(0.0f, 0.0f, 0.0f);
        
        for (unsigned int i = 0; i < dtm->triangle_count; i++) {
            Triangle t = dtm->triangles[i];
            glVertex3f(dtm->points[t.p1].x, dtm->points[t.p1].y, dtm->points[t.p1].z);
            glVertex3f(dtm->points[t.p2].x, dtm->points[t.p2].y, dtm->points[t.p2].z);
            glVertex3f(dtm->points[t.p3].x, dtm->points[t.p3].y, dtm->points[t.p3].z);
        }

        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 1;
}

