#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <omp.h>

// Sequential Bubble Sort
void bubbleSort(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(int arr[], int size)
{
    int i, j;
#pragma omp parallel for private(i, j) shared(arr)
    for (i = 0; i < size - 1; i++)
    {
        for (j = 0; j < size - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Merge two subarrays into a sorted array
void merge(int arr[], int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    int *leftArr = new int[n1];
    int *rightArr = new int[n2];

    for (i = 0; i < n1; i++)
    {
        leftArr[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++)
    {
        rightArr[j] = arr[middle + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    delete[] leftArr;
    delete[] rightArr;
}

// Sequential Merge Sort
void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            {
                parallelMergeSort(arr, left, middle);
            }

#pragma omp section
            {
                parallelMergeSort(arr, middle + 1, right);
            }
        }

        merge(arr, left, middle, right);
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    const int size = 10000;
    int *arr = new int[size];
    int *arrCopy = new int[size];

    // Generate random array
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % 10000;
    }

    // Make a copy of the array for verification
    std::copy(arr, arr + size, arrCopy);

    double startTime, endTime;

    // Sequential Bubble Sort
    startTime = omp_get_wtime();
    bubbleSort(arr, size);
    endTime = omp_get_wtime();
    std::cout << "Sequential Bubble Sort Time: " << (endTime - startTime) << " seconds" << std::endl;

    // Parallel Bubble Sort
    std::copy(arrCopy, arrCopy + size, arr); // Reset the array to unsorted values
    startTime = omp_get_wtime();
    parallelBubbleSort(arr, size);
    endTime = omp_get_wtime();
    std::cout << "Parallel Bubble Sort Time: " << (endTime - startTime) << " seconds" << std::endl;

    // Sequential Merge Sort
    std::copy(arrCopy, arrCopy + size, arr); // Reset the array to unsorted values
    startTime = omp_get_wtime();
    mergeSort(arr, 0, size - 1);
    endTime = omp_get_wtime();
    std::cout << "Sequential Merge Sort Time: " << (endTime - startTime) << " seconds" << std::endl;

    // Parallel Merge Sort
    std::copy(arrCopy, arrCopy + size, arr); // Reset the array to unsorted values
    startTime = omp_get_wtime();
    parallelMergeSort(arr, 0, size - 1);
    endTime = omp_get_wtime();
    std::cout << "Parallel Merge Sort Time: " << (endTime - startTime) << " seconds" << std::endl;

    // Cleanup
    delete[] arr;
    delete[] arrCopy;

    return 0;
}

//  g++ -fopenmp bubble_sort.cpp -o program
//  ./program