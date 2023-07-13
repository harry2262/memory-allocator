# Memory Allocator

This project is a basic memory allocator implemented in C. It provides functions for dynamically allocating and freeing memory blocks, as well as additional functionalities like `calloc` and `realloc`. The purpose of this project is to showcase my understanding of memory management and demonstrate my ability to implement fundamental memory allocation techniques.

## Features

- `malloc`: Dynamically allocates a block of memory of the specified size.
- `free`: Frees a previously allocated block of memory.
- `calloc`: Allocates and initializes a block of memory for an array, setting all bytes to zero.
- `realloc`: Changes the size of a previously allocated block of memory.
- Thread-safe: The memory allocator is designed to be thread-safe using `pthread_mutex_t` for synchronization.

## Usage

To use the memory allocator in your own projects, follow these steps:

1. Clone the repository or download the source code.
2. Include the necessary header files in your program:
   ```c
   #include "memory_allocator.h"
   ```
3. Compile your program along with the memory allocator source files:
   ```bash
   gcc your_program.c memory_allocator.c -o your_program -lpthread
   ```
4. Use the memory allocator functions in your code, such as `malloc`, `free`, `calloc`, and `realloc`, to manage memory dynamically.
5. Make sure to properly handle any errors that may occur during memory allocation.
6. If needed, refer to the provided `print_mem_list` function for debugging or monitoring the memory allocation status.

## Examples

Here's a simple example demonstrating the usage of the memory allocator functions:

```c
#include <stdio.h>
#include "memory_allocator.h"

int main() {
    // Allocate memory
    int* ptr = (int*)malloc(5 * sizeof(int));

    if (ptr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Use the allocated memory
    for (int i = 0; i < 5; i++) {
        ptr[i] = i + 1;
    }

    // Print the allocated values
    for (int i = 0; i < 5; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    // Free the memory
    free(ptr);

    return 0;
}
```

## Contribution

Contributions to the project are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## Acknowledgments

This project is inspired by the concepts and techniques of memory management in C. I would like to acknowledge the valuable resources and documentation available on memory allocation techniques and best practices that contributed to the development of this memory allocator.

## Contact

For any further questions or inquiries, please reach out to [mail](mailto:jashanbath9@gmail.com).

Thank you for your interest in this memory allocator project!
