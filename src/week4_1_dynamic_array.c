/*
 * week4_1_dynamic_array.c
 * Author: Ege Engin
 * Student ID: 260ADM039
 * Description:
 *   Demonstrates creation and usage of a dynamic array using malloc.
 *   Allocate memory for n integers, read them from the user,
 *   print their sum and average, and then free the memory.
 *
 *   Output must match the format in the Week 4 instructions exactly
 *   (it is checked by the autograder).
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    int *arr = NULL;

    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid size.\n");
        return 1;
    }

    // Allocate dynamic memory for 'n' integers based on the size of an int
    arr = malloc(n * sizeof(int));
    // Example: arr = malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    // if arr is NULL: print "Memory allocation failed." and return 1

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        // reading input and checking if the user entered a valid integer or not
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input.\n");
            free(arr);
            return 1;
        }
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    // cast to float to avoid integer truncation during division
    float average = (float)sum / n;

    // formatted output for re
    printf("Sum = %d\n", sum);
    printf("Average = %.2f\n", average);

    // release the dynamically allocated memory back to the system
    free(arr);
    return 0;
}
