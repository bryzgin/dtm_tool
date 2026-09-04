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

