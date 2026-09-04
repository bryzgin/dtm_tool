/**
 * @file triangulation.c
 * @brief definition of the Delaunay triangulation functions
 */

#include "triangulation.h"
#include <math.h>
#include <stdlib.h>

static Circle get_circumcircle(Point a, Point b, Point c)
{
    Circle circle;

    float d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));

    if (fabsf(d) < 0.000001f) {
        circle.center_x = 0.0f;
        circle.center_y = 0.0f;
        circle.radius = -1.0f;
        return circle;
    }

    float a_sq = a.x * a.x + a.y * a.y;
    float b_sq = b.x * b.x + b.y * b.y;
    float c_sq = c.x * c.x + c.y * c.y;

    circle.center_x = (a_sq * (b.y - c.y) + b_sq * (c.y - a.y) + c_sq * (a.y - b.y)) / d;
    circle.center_y = (a_sq * (c.x - b.x) + b_sq * (a.x - c.x) + c_sq * (b.x - a.x)) / d;
    
    float dx = a.x - circle.center_x;
    float dy = a.y - circle.center_y;
    circle.radius = sqrtf(dx * dx + dy * dy);

    return circle;
}

static int is_point_in_circumcircle(Point p, Point a, Point b, Point c)
{
    Circle circle = get_circumcircle(a, b, c);

    if (circle.radius < 0.0f) {
        return 0;
    }

    float dx = p.x - circle.center_x;
    float dy = p.y - circle.center_y;
    float distance = sqrtf(dx * dx + dy * dy);

    return (distance < (circle.radius - 0.000001f));
}

int triangulate_delaunay(DTM* dtm)
{
    if (dtm == NULL || dtm->point_count < 3) {
        return 0;
    }

    float min_x = dtm->points[0].x;
    float min_y = dtm->points[0].y;
    float max_x = dtm->points[0].x;
    float max_y = dtm->points[0].y;

    for (unsigned int i = 1; i < dtm->point_count; i++) {
        if (dtm->points[i].x < min_x) min_x = dtm->points[i].x;
        if (dtm->points[i].y < min_y) min_y = dtm->points[i].y;
        if (dtm->points[i].x > max_x) max_x = dtm->points[i].x;
        if (dtm->points[i].y > max_y) max_y = dtm->points[i].y;
    }

    float dx = max_x - min_x;
    float dy = max_y - min_y;
    float delta_max = (dx > dy) ? dx : dy;
    float mid_x = (min_x + max_x) * 0.5f;
    float mid_y = (min_y + max_y) * 0.5f;

    if (dtm->point_count + 3 > dtm->point_capacity) {
        unsigned int new_capacity = (dtm->point_capacity == 0) ? 128 : dtm->point_capacity * 2;

        while (dtm->point_count + 3 > new_capacity) {
            new_capacity *= 2;
        }
        
        Point* temp = (Point*)realloc(dtm->points, new_capacity * sizeof(Point));
        if (temp == NULL) return 0;
        dtm->points = temp;
        dtm->point_capacity = new_capacity;
    }

    unsigned int st_p1 = dtm->point_count;
    unsigned int st_p2 = dtm->point_count + 1;
    unsigned int st_p3 = dtm->point_count + 2;

    dtm->points[st_p1].x = mid_x - 20.0f * delta_max;
    dtm->points[st_p1].y = mid_y - delta_max;
    dtm->points[st_p1].z = 0.0f;
    
    dtm->points[st_p2].x = mid_x;
    dtm->points[st_p2].y = mid_y + 20.0f * delta_max;
    dtm->points[st_p2].z = 0.0f;

    dtm->points[st_p3].x = mid_x + 20.0f * delta_max;
    dtm->points[st_p3].y = mid_y - delta_max;
    dtm->points[st_p3].z = 0.0f;

    dtm->point_count += 3;

    if (dtm->triangle_capacity == 0) {
        dtm->triangle_capacity = 128;
        dtm->triangles = (Triangle*)malloc(dtm->triangle_capacity * sizeof(Triangle));
        if (dtm->triangles == NULL) return 0;
    }
    
    dtm->triangles[0].p1 = st_p1;
    dtm->triangles[0].p2 = st_p2;
    dtm->triangles[0].p3 = st_p3;
    dtm->triangle_count = 1;

    return 1;
}

