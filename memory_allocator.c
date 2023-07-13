#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

typedef char ALLIGN[16];

// Define the header structure
union header {
    struct {
        size_t size;
        unsigned is_free;
        union header* next;
    } s;
    ALLIGN stub;
};

typedef union header header_t;
header_t* head = NULL;
header_t* tail = NULL;
pthread_mutex_t global_malloc_lock;

// Find a free block of size >= 'size'
header_t* get_free_block(size_t size) {
    header_t* curr = head;
    while (curr) {
        if (curr->s.is_free && curr->s.size >= size) {
            return curr;
        }
        curr = curr->s.next;
    }
    return NULL;
}

// Allocate memory
void* malloc(size_t size) {
    size_t total_size;
    header_t* header;
    void* block;

    if (!size) {
        return NULL;
    }

    pthread_mutex_lock(&global_malloc_lock);

    // Check if a free block is available
    header = get_free_block(size);
    if (header) {
        header->s.is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header + 1);
    }

    total_size = sizeof(header_t) + size;
    block = sbrk(total_size);

    if (block == (void*)-1) {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = block;
    header->s.is_free = 0;
    header->s.size = size;
    header->s.next = NULL;

    if (!head) {
        head = header;
    }

    if (tail) {
        tail->s.next = header;
    }

    tail = header;

    pthread_mutex_unlock(&global_malloc_lock);

    return (void*)(header + 1);
}

// Free memory
void free(void* block) {
    header_t* header, * tmp;
    void* programbreak;

    if (!block) {
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);

    header = (header_t*)block - 1;

    programbreak = sbrk(0);
    if ((char*)block + header->s.size == programbreak) {
        if (head == tail) {
            head = tail = NULL;
        }
        else {
            tmp = head;
            while (tmp) {
                if (tmp->s.next == tail) {
                    tmp->s.next = NULL;
                    tail = tmp;
                }
                tmp = tmp->s.next;
            }
        }
        sbrk(0 - sizeof(header_t) - header->s.size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }

    header->s.is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}

// Calloc function
void* calloc(size_t num, size_t nsize) {
    size_t size;
    void* block;

    if (!num || !nsize) {
        return NULL;
    }

    size = num * nsize;

    /* Check for multiplication overflow */
    if (nsize != size / num) {
        return NULL;
    }

    block = malloc(size);

    if (!block) {
        return NULL;
    }

    memset(block, 0, size);

    return block;
}

// Realloc function
void* realloc(void* block, size_t size) {
    header_t* header;
    void* ret;

    if (!block || !size) {
        return malloc(size);
    }

    header = (header_t*)block - 1;

    if (header->s.size >= size) {
        return block;
    }

    ret = malloc(size);
    if (ret) {
        memcpy(ret, block, header->s.size);
        free(block);
    }

    return ret;
}

// Print memory block information
void print_mem_list() {
    header_t* curr = head;
    printf("head = %p, tail = %p\n", (void*)head, (void*)tail);
    while (curr) {
        printf("addr = %p, size = %zu, is_free = %u, next = %p\n",
            (void*)curr, curr->s.size, curr->s.is_free, (void*)curr->s.next);
        curr = curr->s.next;
    }
}
