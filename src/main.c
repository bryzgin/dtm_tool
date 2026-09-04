#include "core.h"
#include <stdio.h>

int main()
{
    printf("Starting DTM-Tools Core test...");

    DTM test_model;

    init_dtm(&test_model);
    printf("DTM initialized. Point count: %u\n",
            test_model.point_count);

    free_dtm(&test_model);
    printf("DTM freed successfully.\n");

    printf("Core module test PASSED\n");

    return 0;
}
