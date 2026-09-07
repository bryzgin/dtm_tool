#include "viewer.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>


static float rotation_x = 60.0f;
static float rotation_z = 0.0f;


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

static void calculate_normal(Point p1, Point p2, Point p3, float* nx, float* ny, float* nz)
{
    float ux = p2.x - p1.x;
    float uy = p2.y - p1.y;
    float uz = p2.z - p1.z;

    float vx = p3.x - p1.x;
    float vy = p3.y - p1.y;
    float vz = p3.z - p1.z;

    float crx = uy * vz -uz * vy;
    float cry = uz * vx - ux * vz;
    float crz = ux * vy * vx;

    float length = sqrtf(crx * crx + cry + crz * crz);
    if (length > 0.00001f) {
        *nx = crx / length;
        *ny = cry / length;
        *nz = crz / length;
    } else {
        *nx = 0.0f; *ny = 0.0f; *nz = 1.0f;
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
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "DTM TOOLS 3D Viewer", NULL, NULL);
    if (!window) {
        printf("Error: Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_NORMALIZE);

    float light_ambient[] = {0.2f , 0.2f, 0.2f, 1.0f};
    float light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float light_position[] = {1.0f, 1.0f, 2.0f, 0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    float mat_specular[] = {0.6f, 0.6f, 0.6f, 1.0f};
    float mat_shiness[] = {64.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shiness);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

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
    float scale = (max_dim > 0.0001f) ? (1.5f / max_dim) : 1.0f;
    
    float z_range = (max_z - min_z > 0.0001f) ? (max_z - min_z) : 1.0f;

    float center_x = (min_x + max_x) * 0.5f;
    float center_y = (min_y + max_y) * 0.5f;
    float center_z = (min_z + max_z) * 0.5f;

    printf("Rendering 3D scene\n");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glLoadIdentity();
        glScalef(scale, scale, -scale);

        glRotatef(-rotation_x, 1.0f, 0.0f, 0.0f);
        glRotatef(rotation_z, 0.0f, 0.0f, 1.0f);

        glTranslatef(-center_x, -center_y, -center_z);
        
        float* vertex_normals = (float*)calloc(dtm->point_count * 3, sizeof(float));
        for (unsigned int i = 0; i < dtm->triangle_count; i++) {
            Triangle t = dtm-> triangles[i];
            Point p1 = dtm->points[t.p1];
            Point p2 = dtm->points[t.p2];
            Point p3 = dtm->points[t.p3];

            float nx, ny, nz;
            calculate_normal(p1, p2, p3, &nx, &ny, &nz);

            vertex_normals[t.p1 * 3 + 0] += nx; vertex_normals[t.p1 * 3 + 1] += ny; vertex_normals[t.p1 * 3 + 2] += nz;
            vertex_normals[t.p2 * 3 + 0] += nx; vertex_normals[t.p2 * 3 + 1] += ny; vertex_normals[t.p2 * 3 + 2] += nz;
            vertex_normals[t.p3 * 3 + 0] += nx; vertex_normals[t.p3 * 3 + 1] += ny; vertex_normals[t.p3 * 3 + 2] += nz;
        }

        for (unsigned int i = 0; i < dtm->point_count; i++) {
            float nx = vertex_normals[i * 3 + 0];
            float ny = vertex_normals[i * 3 + 1];
            float nz = vertex_normals[i * 3 + 2];
            float len = sqrt(nx * nx + ny * ny + nz * nz);
            
            if (len > 0.00001f) {
                vertex_normals[i * 3 + 0] /= len;
                vertex_normals[i * 3 + 1] /= len;
                vertex_normals[i * 3 + 2] /= len;
            }
        }

        glBegin(GL_TRIANGLES);

        for (unsigned int i = 0; i < dtm->triangle_count; i++) {
            Triangle t = dtm->triangles[i];

            Point p1 = dtm->points[t.p1];
            Point p2 = dtm->points[t.p2];
            Point p3 = dtm->points[t.p3];

            float factor1 = (p1.z - min_z) / z_range;
            glColor3f(0.1f, 0.3f + factor1 * 0.7f, 0.1f);
            glNormal3f(vertex_normals[t.p1 * 3 + 0], vertex_normals[t.p1 * 3 + 1], vertex_normals[t.p1 * 3 + 2]);
            glVertex3f(p1.x, p1.y, p1.z);

            float factor2 = (p2.z - min_z) / z_range;
            glColor3f(0.1f, 0.3f + factor2 * 0.7f, 0.1f);
            glNormal3f(vertex_normals[t.p2 * 3 + 0], vertex_normals[t.p2 * 3 + 1], vertex_normals[t.p2 * 3 + 2]);
            glVertex3f(p2.x, p2.y, p2.z);

            float factor3 = (p3.z - min_z) / z_range;
            glColor3f(0.1f, 0.3f + factor3 * 0.7f, 0.1f);
            glNormal3f(vertex_normals[t.p3 * 3 + 0], vertex_normals[t.p3 * 3 + 1], vertex_normals[t.p3 * 3 + 2]);
            glVertex3f(p3.x, p3.y, p3.z);
        }
        glEnd();

        free(vertex_normals);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 1;
}

