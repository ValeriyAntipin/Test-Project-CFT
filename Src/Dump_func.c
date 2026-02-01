#include "Dump_func.h"

/**
 * @brief чтение дампа данных
 * @param par_path путь до файла
 * @param par_array указатель на указатель массива StatData
 * @param par_len кол-во объектов из заголовка 
 * @return код ошибки
 */
uint32_t LoadDump(const char *par_path, StatData **par_array, size_t *par_len){
    uint32_t loc_u32count = 0;
    int64_t loc_id = 0;
    int32_t loc_count = 0;
    float loc_cost = 0; 
    uint8_t loc_flags = 0;
    StatData *loc_statData_buf = NULL;
    FILE *load_file = fopen(par_path, "rb");

    if (!load_file){
        return OPEN_FILE_ERROR;
    }

    if (fread(&loc_u32count, sizeof(loc_u32count), 1, load_file) != 1){
        fclose(load_file);
        return READ_HEADER_ERROR;
    }

    if (loc_u32count > 0)
    {
        loc_statData_buf = malloc(loc_u32count * sizeof(StatData));
        
        if (!loc_statData_buf){
            fclose(load_file);
            return MEMORY_ALLOC_ERROR;
        }
    }

    for (size_t i = 0; i < loc_u32count; i++){
        
        if (fread(&loc_id, sizeof(loc_id), 1, load_file) != 1 ||
            fread(&loc_count, sizeof(loc_count), 1, load_file) != 1 ||
            fread(&loc_cost, sizeof(loc_cost), 1, load_file) != 1 ||
            fread(&loc_flags, sizeof(loc_flags), 1, load_file) != 1){
                free(loc_statData_buf);
                fclose(load_file);
                return READ_DATA_ERROR;
            }
        
        loc_statData_buf[i].id = (long)loc_id;
        loc_statData_buf[i].count = (int)loc_count;
        loc_statData_buf[i].cost = loc_cost;
        loc_statData_buf[i].primary = loc_flags & 0x01;
        loc_statData_buf[i].mode = (loc_flags >> 1) & 0x07;
    }

    if (fclose(load_file) != 0){
        free(loc_statData_buf);
        return CLOSE_FILE_ERROR;
    }

    *par_len = loc_u32count;
    *par_array = loc_statData_buf;

    return NO_ERROR;
}

/**
 * @brief запись дампа данных в файл, первые 4 байта файла - кол-во элем. файла
 * @param par_path путь до файла, куда записываем
 * @param par_array указатель на массив структур 
 * @param par_len кол-во элем. для заголовка
 * @return код ошибки 
 */
uint32_t StoreDump(const char *par_path, StatData *par_array, size_t par_len){
    uint32_t loc_u32count = 0;
    int64_t loc_id = 0;
    int32_t loc_count = 0;
    float loc_cost = 0; 
    FILE *store_file = fopen(par_path, "wb");
    
    if (!store_file){
        fclose(store_file);
        return OPEN_FILE_ERROR;
    }

    loc_u32count = (uint32_t)par_len;

    if (fwrite(&loc_u32count, sizeof(loc_u32count), 1, store_file) != 1){
        fclose(store_file);
        return WRITE_HEADER_ERROR;
    }

   for (size_t i = 0; i < loc_u32count; i++){
        uint8_t loc_flags = 0;
        loc_id = (int64_t)par_array[i].id;
        loc_count = (int32_t)par_array[i].count;
        loc_cost = par_array[i].cost;
        
        if (par_array[i].primary){
            loc_flags |= 0x01;
        }

        loc_flags |= (uint8_t)((par_array[i].mode & 0x07) << 1);
        if (fwrite(&loc_id, sizeof(loc_id), 1, store_file) != 1 ||
            fwrite(&loc_count, sizeof(loc_count), 1, store_file) != 1 ||
            fwrite(&loc_cost, sizeof(loc_cost), 1, store_file) != 1 ||
            fwrite(&loc_flags, sizeof(loc_flags), 1, store_file) != 1){
                fclose(store_file);
                return WRITE_DATA_ERROR;
            }   
    }

    fclose(store_file);
    return NO_ERROR;
}

static int compare_id(const void *par_struct1, const void *par_struct2){
    StatData *loc_struct1 = (StatData*)par_struct1;
    StatData *loc_struct2 = (StatData*)par_struct2;

    if (loc_struct1->id < loc_struct2->id) return -1;
    if (loc_struct1->id > loc_struct2->id) return 1;
    return 0;
}

/**
 * @brief объединение двух массивов StatData
 * @param par_array1 
 * @param par_array2 
 * @param par_len1 
 * @param par_len2 
 * @param par_result_array 
 * @param par_res_len 
 * @return код ошибки
 */
uint32_t JoinDump(const StatData *par_array1, const StatData *par_array2, size_t par_len1, size_t par_len2, StatData **par_result_array,  size_t *par_res_len){
    uint32_t loc_buffer_size = 0;
    StatData *loc_tmp_statData = NULL;
    StatData *loc_result_statData = NULL;
    size_t loc_step = 0;

    if (!par_result_array || !par_res_len){
        return ZERO_POINET;
    }

    loc_buffer_size = par_len1 + par_len2;
    
    if (!loc_buffer_size){
        return EMPTY_BUFFER;
    }

    loc_tmp_statData = malloc(loc_buffer_size * sizeof(StatData));

    if (!loc_tmp_statData){
        return MEMORY_ALLOC_ERROR;
    }

    for (size_t i = 0; i < par_len1; i++){
        loc_tmp_statData[i] = par_array1[i];
    }
    for (size_t i = 0; i < par_len2; i++){
        loc_tmp_statData[par_len1 + i] = par_array2[i];
    }

    qsort(loc_tmp_statData, loc_buffer_size, sizeof(StatData), compare_id);

    loc_result_statData = malloc(loc_buffer_size * sizeof(StatData));

    if (!loc_result_statData){
        free(loc_tmp_statData);
        return MEMORY_ALLOC_ERROR;
    }

    loc_result_statData[loc_step] = loc_tmp_statData[loc_step];

    for (size_t i = 1; i < loc_buffer_size; i++){

        if (loc_tmp_statData[i].id == loc_result_statData[loc_step].id){
            loc_result_statData[loc_step].count += loc_tmp_statData[i].count;
            loc_result_statData[loc_step].cost += loc_tmp_statData[i].cost;
            loc_result_statData[loc_step].primary &= loc_tmp_statData[i].primary;

            if (loc_result_statData[loc_step].mode < loc_tmp_statData[i].mode){
                loc_result_statData[loc_step].mode = loc_tmp_statData[i].mode;
            }
        } else {
            loc_step++;
            loc_result_statData[loc_step] = loc_tmp_statData[i];
        }
    }

    free(loc_tmp_statData);
    *par_res_len = loc_step + 1;
    *par_result_array = loc_result_statData;
    return NO_ERROR;
}


static int compare_cost(const void *par_struct1, const void *par_struct2){
    StatData *loc_struct1 = (StatData*)par_struct1;
    StatData *loc_struct2 = (StatData*)par_struct2;

    if (loc_struct1->cost < loc_struct2->cost) return -1;
    if (loc_struct1->cost > loc_struct2->cost) return 1;
    return 0;
}

/**
 * @brief функция для сортировки по cost
 * @param par_array указатель на массив структур
 * @param par_len кол-во 
 */
void SortDump(StatData *par_array, size_t par_len){
    qsort(par_array, par_len, sizeof(StatData), compare_cost);
}