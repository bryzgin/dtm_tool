/**
 * @file viewer.h
 * @brief visualization functions
 */

#ifndef VIEWER_H
#define VIEWER_H

#include "core.h"

/**
 * @brief initializes the graphical window
 * @param dtm DTM pointer
 * @return int 1 in case of success, 0 in case an error
 */
int run_viewer(DTM* dtm);

#endif

