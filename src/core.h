/**
 * @file core.c
 * @brief definition of basic functions
 */

#include "core.h"
#include <stdlib.h>

void init_dtm(DTM* dtm)
{
    if (dtm == NULL) {
        return;
    }

    dtm->points = NULL;
    dtm->point_count = 0;
    dtm->point_capacity = 0;

    dtm->triangles = NULL;
    dtm->triangle_count = 0;
    dtm->triangle_capacity = 0;
}

void free_dtm(DTM* dtm) {
    if (dtm == NULL) {
        return;
    }

    if (dtm->points != NULL) {
        free(dtm->points);
    }

    if (dtm->triangles != NULL) {
        free(dtm->triangles);
    }

    init_dtm(dtm);
}
