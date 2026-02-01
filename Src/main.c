#include "Dump_func.h"

static void print_mode(const int par_mode){
    for (size_t i = 2; i > 0; i--){
        putchar((par_mode & (1u << i)) ? '1' : '0');
    }
}

static void print_result(const StatData *par_array){
    printf("0x%lx %d %.3e %c ",
    par_array->id,
    par_array->count,
    par_array->cost,
    par_array->primary ? 'y' : 'n');
    print_mode(par_array->mode);
    putchar('\n');
}

int main(int argc, char *argv[])
{
    size_t loc_count_print = 0;
    StatData *loc_a = NULL, *loc_b = NULL, *loc_res = NULL;
    size_t loc_len_a = 0, loc_len_b = 0, loc_len_res = 0;

    if (LoadDump(argv[1], &loc_a, &loc_len_a) != NO_ERROR){
        fprintf(stderr, "Erorr loading %s\n", argv[1]);
        return ERROR_LOAD;
    }

    if (LoadDump(argv[2], &loc_b, &loc_len_b) != NO_ERROR){
        fprintf(stderr, "Erorr loading %s\n", argv[2]);
        free(loc_a);
        return ERROR_LOAD;
    }

    if (JoinDump(loc_a, loc_b, loc_len_a, loc_len_b, &loc_res, &loc_len_res) != NO_ERROR){
        fprintf(stderr, "Erorr join data\n");
        free(loc_a);
        free(loc_b);
        return ERROR_JOIN;
    }

    free(loc_a);
    free(loc_b);

    SortDump(loc_res, loc_len_res);

    loc_count_print = loc_len_res < 10 ? loc_len_res : 10;
    for (size_t i = 0; i < loc_count_print; i++){
        print_result(&loc_res[i]);
    }

    if (StoreDump(argv[3], loc_res, loc_len_res) != NO_ERROR){
        fprintf(stderr, "Error writing result file\n");
        free(loc_res);
        return ERROR_WRITE;
    }

    free (loc_res);
    return NO_ERROR;
}