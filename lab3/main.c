#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void InitializeArray(int *array, long from, long to) {
    for (int i = from; i < to; i++) {
        array[i] = i;
    }
}

void InitializeArrayByBlocks(int *array, long array_size, int *block, int block_size) {
    int count_of_blocks = array_size / block_size;
    for (int i = 0; i <= count_of_blocks; i++) {
        memcpy(array + i * block_size, block, block_size * sizeof(int));
    }
}

int* CreateArray(long sizeBytes) {
    int *array = malloc(sizeBytes);
    if (array == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }
    return array;
}

int* MultiplyArraySize(int *array, long *size, float multiplier) {
    long new_size = *size * multiplier;
    int *temp = realloc(array, new_size * sizeof(int));
    if (temp == NULL) {
        printf("Memory allocation error\n");
        return array;
    }
    *size = new_size;
    return temp;
}

int* IncrementArray(int *array, long *size, int incrementBytes) {
    long new_size = *size + incrementBytes / sizeof(int);
    int *temp = realloc(array, new_size * sizeof(int));
    if (temp == NULL) {
        printf("Memory allocation error\n");
        return array;
    }
    *size = new_size;
    return temp;
}

int main(void) {
    clock_t start, end;
    double elapsed_time;
    long array_size, old_size;
    int start_size;

    printf("Enter the size of the array in MB: ");
    fflush(stdout);
    scanf("%d", &start_size);

    start = clock();
    int *array = CreateArray(start_size * 1024 * 1024);
    printf("Array pointer - %p\n", array);

    array_size = (start_size * 1024 * 1024) / sizeof(int);

    printf("Incrementing array size by multiplying...\n");
    old_size = array_size;
    for (int i = 0; i < 5; i++) {
        array = MultiplyArraySize(array, &array_size, 1.5);
    }
    printf("Array size after multiplication - %ld integers\n", array_size);
    printf("Array pointer - %p\n", array);
    InitializeArray(array, old_size, array_size);

    end = clock();
    elapsed_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("Memory reallocation completed in %.3f milliseconds\n", elapsed_time);

    int block_size = 1048576 / sizeof(int);
    int *block = malloc(block_size * sizeof(int));
    for (int i = 0; i < block_size; i++) {
        block[i] = i;
    }

    start = clock();
    InitializeArrayByBlocks(array, array_size, block, block_size);

    end = clock();
    elapsed_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    printf("Block-based initialization completed in %.3f milliseconds\n", elapsed_time);

    free(array);
    free(block);

    return 0;
}
