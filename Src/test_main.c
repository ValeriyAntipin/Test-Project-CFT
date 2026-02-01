#include "Dump_func.h"
#include <time.h>

static int compare_arrays(const StatData *par_a, const StatData *par_b, size_t par_len)
{
    for (size_t i = 0; i < par_len; ++i) {
        if (par_a[i].id != par_b[i].id ||
            par_a[i].count != par_b[i].count ||
            par_a[i].cost != par_b[i].cost ||
            par_a[i].primary != par_b[i].primary ||
            par_a[i].mode != par_b[i].mode)
            return -1;
    }
    return 0;
}

int main(void)
{
    clock_t loc_start = clock();

    StatData loc_case_A[2] = {
        {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3},
        {.id = 90089, .count = 1, .cost = 88.90, .primary = 1, .mode = 0}
    };

    StatData loc_case_B[2] = {
        {.id = 90089, .count = 13, .cost = 0.011, .primary = 0, .mode = 2},
        {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2}
    };

    StatData loc_expected[3] = {
        {.id = 90189, .count = 1000, .cost = 1.00003, .primary = 1, .mode = 2 },
        {.id = 90889, .count = 13, .cost = 3.567, .primary = 0, .mode = 3 },
        {.id = 90089, .count = 14, .cost = 88.911, .primary = 0, .mode = 2 }
    };

    StatData *loc_res_array = NULL;
    size_t loc_res_len = 0;

    size_t par_len_A = sizeof(loc_case_A) / sizeof(loc_case_A[0]);
    size_t par_len_B = sizeof(loc_case_B) / sizeof(loc_case_B[0]);

    if (StoreDump("test_a.dump", loc_case_A, par_len_A) != 0 ||
        StoreDump("test_b.dump", loc_case_B, par_len_B) != 0) {
        fprintf(stderr, "StoreDump failed\n");
        return 1;
    }

    if (system("./prog test_a.dump test_b.dump result.dump") != 0) {
        fprintf(stderr, "Main program failed\n");
        return 1;
    }

    if (LoadDump("result.dump", &loc_res_array, &loc_res_len) != 0) {
        fprintf(stderr, "LoadDump result failed\n");
        return 1;
    }

    if (loc_res_len != 3 || compare_arrays(loc_res_array, loc_expected, 3) != 0) {
        fprintf(stderr, "Test 1 FAILED\n");
        free(loc_res_array);
        return 1;
    }

    free(loc_res_array);

    clock_t loc_end = clock();
    double t = (double)(loc_end - loc_start) / CLOCKS_PER_SEC;

    printf("All tests passed. Time: %.3f sec\n", t);
    return 0;
}
