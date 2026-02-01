#ifndef _DUMP_FUNC_
#define _DUMP_FUNC_

#include "Types.h"

uint32_t LoadDump(const char *par_path,  StatData **par_array, size_t *par_len);
uint32_t StoreDump(const char *par_path,  StatData *par_array, size_t par_len);
uint32_t JoinDump(const StatData *par_array1, const StatData *par_array2, size_t par_len1, size_t par_len2, StatData **par_result_array, size_t *par_res_len);
void SortDump(StatData *par_array, size_t par_len);
#endif