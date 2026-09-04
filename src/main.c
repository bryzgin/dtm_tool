#include "core.h"
#include "csv_parser.h"
#include <stdio.h>

int main()
{
    printf("Starting DTM-Tools CSV-parser test...\n");

    DTM test_model;
    init_dtm(&test_model);

    const char* test_csv_file = "data/test_csv_file.csv";
    printf("Loading file: %s\n", test_csv_file);

    if (read_csv(test_csv_file, &test_model)) {
        printf("Successfully read CSV file!\n");
        printf("Total points loaded: %u\n", test_model.point_count);

        printf("--- Loaded points ---\n");
        for (unsigned int i = 0; i < test_model.point_count; i++) {
            printf("Point [%u]: x = %.2f, y = %.2f, z = %.2f\n",
                    i,
                    test_model.points[i].x,
                    test_model.points[i].y,
                    test_model.points[i].z);
        }
        printf("--------------------\n");

    } else {
        printf("ERROR: Failed to read CSV file\n");
    }

    free_dtm(&test_model);
    printf("DTM freed successfully\n");
    printf("CSV-parser test PASSED\n");

    return 0;
}

