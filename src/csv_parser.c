/**
 * @file csv_parser.c
 * @brief definition of functions for parsing 3D points
 */

#include "csv_parser.h"
#include <stdio.h>
#include <stdlib.h>

int read_csv(const char* filename, DTM* dtm)
{
    if (filename == NULL || dtm == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char header_buffer[256];
    if (fgets(header_buffer, sizeof(header_buffer), file) == NULL) {
        fclose(file);
        return 0;
    }

    if (dtm->point_capacity == 0) {
        dtm->point_capacity = 128;
        dtm->points = (Point*)malloc(dtm->point_capacity * sizeof(Point));
        if (dtm->points == NULL) {
            fclose(file);
            return 0;
        }
    }

    float x, y, z;

    while (fscanf(file, "%f,%f,%f\n", &x, &y, &z) == 3) {
        if (dtm->point_count >= dtm->point_capacity) {
            unsigned int new_capacity = dtm->point_capacity * 2;
            Point* temp = (Point*)realloc(dtm->points, new_capacity * sizeof(Point));
            
            if (temp == NULL) {
                fclose(file);
                return 0;
            }

            dtm->points = temp;
            dtm->point_capacity = new_capacity;
        }

        dtm->points[dtm->point_count].x = x;
        dtm->points[dtm->point_count].y = y;
        dtm->points[dtm->point_count].z = z;
        dtm->point_count++;
    }

    fclose(file);
    
    return 1;
}

