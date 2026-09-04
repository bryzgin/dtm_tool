/**
 * @file triangulation.h
 * @brief Delaunay triangulation algorithms
 */

#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "core.h"

/*
 * @struct Circle
 * @brief additional structure for the circle
 */
typedef struct {
    float center_x;
    float center_y;
    float radius;
} Circle;

/**
 * @brief build a Delaunay triangulation
 * @param dtm DTM pointer
 * @return int 1 in case of success, 0 in case of a memory allocation error
 */
int triangulate_delaunay(DTM* dtm);

#endif
 
