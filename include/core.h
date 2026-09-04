/**
 * @file core.h
 * @brief basic structures of geometric objects and memory management
 */

#ifndef CORE_H
#define CORE_H

/**
 * @struct Point
 * @brief 3D point structure
 */
typedef struct {
    float x;
    float y;
    float z;
} Point;

/**
 * @struct Triangle
 * @brief triangle structure with point indices
 */
typedef struct {
    unsigned int p1;
    unsigned int p2;
    unsigned int p3;
} Triangle;

/**
 * @struct DTM
 * @brief digital terrain model container
 */
typedef struct {
    Point* points;
    unsigned int point_count;
    unsigned int point_capacity;

    Triangle* triangles;
    unsigned int triangle_count;
    unsigned int triangle_capacity;
} DTM;

/**
 * @brief initializes DTM structure
 * @param dtm pointer
 */
void init_dtm(DTM* dtm);

/**
 * @brief frees dynaminc memory of the DTM structure
 * @param dtm pointer
 */
void free_dtm(DTM* dtm);

#endif
