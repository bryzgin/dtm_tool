/**
 * @file csv_parser.h
 * @brief functions for reading 3D points from csv
 */

#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "core.h"

/**
 * @brief reads 3D points from CSV file 
 *
 * @param filename path for csv file
 * @param dtm pointer for DTM structure
 * @return int returns 1 if the reading successful, 0 if an error occurs
 */
int read_csv(const char* filename, DTM* dtm);

#endif

