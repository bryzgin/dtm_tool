#include "viewer.h"
#include "core.h"
#include "csv_parser.h"
#include "triangulation.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: %s <path_to_csv_file>\n", argv[0]);
        printf("Example: %s data/test_csv_file.csv\n", argv[0]);
        return -1;
    }

    printf("Starting DTM-Tools Pipeline...\n");

    DTM test_model;
    init_dtm(&test_model);

    const char* test_csv_file = argv[1];
    printf("Loading CSV file: %s\n", test_csv_file);

    if (!read_csv(test_csv_file, &test_model)) {
        printf("Error: Failed to read CSV file\n");
        free_dtm(&test_model);
        return -1;
    }
    printf("Points loaded successfully: %u\n", test_model.point_count);

    printf("Running Delaunay triangulation\n");
    if (!triangulate_delaunay(&test_model)) {
        printf("Error: Triangulation failed\n");
        free_dtm(&test_model);
        return -1;
    }
    printf("Triangulation finished successfully\n");
    printf("Total triangles generated: %u\n", test_model.triangle_count);

    printf("Launching 3D Viewer\n");
    if (!run_viewer(&test_model)) {
        printf("Error: 3D Viewer failed to run\n");
    }

    free_dtm(&test_model);
    printf("DTM-Tools execution finished\n");

    return 0;
}

