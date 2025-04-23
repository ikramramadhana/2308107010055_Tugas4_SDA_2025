// main.c
// Menguji performa algoritma sorting dengan data dari file (angka dan kata)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"

#define MAX_SIZE 2000000

void load_data(const char *filename, int data[], int count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Gagal membuka file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < count; i++) {
        fscanf(fp, "%d", &data[i]);
    }
    fclose(fp);
}

void load_data_string(const char *filename, char **data, int count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Gagal membuka file kata");
        exit(EXIT_FAILURE);
    }
    char buffer[100];
    for (int i = 0; i < count; i++) {
        fscanf(fp, "%s", buffer);
        data[i] = strdup(buffer);
    }
    fclose(fp);
}

void free_string_data(char **data, int count) {
    for (int i = 0; i < count; i++) {
        free(data[i]);
    }
}

void test_sort(void (*sortFunc)(int[], int), int data[], int n, const char *name) {
    int *copy = malloc(n * sizeof(int));
    if (!copy) {
        perror("Gagal alokasi memori");
        return;
    }
    for (int i = 0; i < n; i++) copy[i] = data[i];

    clock_t start = clock();
    sortFunc(copy, n);
    clock_t end = clock();

    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%s: %.3f detik\n", name, duration);

    free(copy);
}

void test_sort_str(void (*sortFunc)(char **, int), char **data, int n, const char *name) {
    char **copy = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        copy[i] = strdup(data[i]);
    }

    clock_t start = clock();
    sortFunc(copy, n);
    clock_t end = clock();

    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%s (kata): %.3f detik\n", name, duration);

    free_string_data(copy, n);
    free(copy);
}

// Wrapper agar mergeSort dan quickSort bisa dipanggil dari pointer fungsi
void mergeSortWrapper(int arr[], int n) {
    mergeSort(arr, 0, n - 1);
}

void quickSortWrapper(int arr[], int n) {
    quickSort(arr, 0, n - 1);
}

int main() {
    int *data = malloc(MAX_SIZE * sizeof(int));
    char **dataStr = malloc(MAX_SIZE * sizeof(char *));
    if (!data || !dataStr) {
        perror("Gagal alokasi memori utama");
        return EXIT_FAILURE;
    }

    int sizes[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
    int num_tests = sizeof(sizes) / sizeof(sizes[0]);

    printf("\n=== UJI DATA ANGKA ===\n");
    load_data("data_angka.txt", data, MAX_SIZE);
    for (int i = 0; i < num_tests; i++) {
        int n = sizes[i];
        printf("\n--- Uji dengan %d data ---\n", n);
        test_sort(bubbleSort, data, n, "Bubble Sort");
        test_sort(selectionSort, data, n, "Selection Sort");
        test_sort(insertionSort, data, n, "Insertion Sort");
        test_sort(mergeSortWrapper, data, n, "Merge Sort");
        test_sort(quickSortWrapper, data, n, "Quick Sort");
        test_sort(shellSort, data, n, "Shell Sort");
    }

    printf("\n=== UJI DATA KATA ===\n");
    load_data_string("data_kata.txt", dataStr, MAX_SIZE);
    for (int i = 0; i < num_tests; i++) {
        int n = sizes[i];
        printf("\n--- Uji dengan %d kata ---\n", n);
        test_sort_str(bubbleSortStr, dataStr, n, "Bubble Sort");
        test_sort_str(selectionSortStr, dataStr, n, "Selection Sort");
        test_sort_str(insertionSortStr, dataStr, n, "Insertion Sort");
        test_sort_str(mergeSortStrWrapper, dataStr, n, "Merge Sort");
        test_sort_str(quickSortStrWrapper, dataStr, n, "Quick Sort");
        test_sort_str(shellSortStr, dataStr, n, "Shell Sort");
    }

    free(data);
    free_string_data(dataStr, MAX_SIZE);
    free(dataStr);
    return 0;
}