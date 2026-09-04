#include "core.h"
#include "csv_parser.h"
#include "triangulation.h"
#include <stdio.h>

int main()
{
    printf("Starting DTM-Tools Pipeline test...\n");

    DTM test_model;
    init_dtm(&test_model);

    const char* test_csv_file = "data/test_csv_file.csv";
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

    printf("Triangles\n");
    unsigned int limit = (test_model.triangle_count < 5) ? test_model.triangle_count : 5;

    for (unsigned int i = 0; i < limit; i++) {
        Triangle t = test_model.triangles[i];
        printf("Triangle [%u]: (%u %u %u)\n", i, t.p1, t.p2, t.p3);
    }

    free_dtm(&test_model);
    printf("Memory cleaned up. Integration pipeline test PASSED\n");

    return 0;
}

