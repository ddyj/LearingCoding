#include <windows.h>
#include <stddef.h>
#include <stdio.h>

struct Block {
    size_t size;   // Block size
    Block* next;   // Pointer to the next free block
    bool free;     // Whether this block is free or not
};

// Head of the free list
Block* freeList = nullptr;

// Aligns size to a multiple of 8 for better memory alignment
size_t align(size_t size) {
    return (size + 7) & ~7;
}

// Function to find a free block with at least `size` bytes
Block* findFreeBlock(Block** last, size_t size) {
    Block* current = freeList;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

// Function to request a new block from the OS
Block* requestSpace(Block* last, size_t size) {
    Block* block = (Block*)VirtualAlloc(NULL, size + sizeof(Block), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!block) {
        return nullptr; // VirtualAlloc failed
    }

    if (last) { // NULL on the first request
        last->next = block;
    }

    block->size = size;
    block->next = nullptr;
    block->free = false;
    return block;
}

// Our implementation of malloc
void* myMalloc(size_t size) {
    Block* block;

    if (size <= 0) {
        return nullptr;
    }

    size = align(size);

    if (!freeList) { // First call
        block = requestSpace(nullptr, size);
        if (!block) {
            return nullptr;
        }
        freeList = block;
    }
    else {
        Block* last = freeList;
        block = findFreeBlock(&last, size);
        if (!block) { // Failed to find a free block
            block = requestSpace(last, size);
            if (!block) {
                return nullptr;
            }
        }
        else { // Found a free block
            block->free = false;
        }
    }
    //block是一个内存空间的描述符 而malloc返回声明的地址是block的后一位
    return (block + 1);
}

// Coalesces adjacent free blocks to prevent fragmentation
// 合并空闲区域
void coalesce() {
    Block* current = freeList;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += current->next->size + sizeof(Block);
            current->next = current->next->next;
        }
        else {
            current = current->next;
        }
    }
}

// Our implementation of free
void myFree(void* ptr) {
    if (!ptr) {
        return;
    }
    //block是一个内存空间的描述符 而malloc返回声明的地址是block的后一位
    //所以在返回析构的时候也会析构后面一位
    Block* block = (Block*)ptr - 1;
    block->free = true;

    // Optionally, coalesce adjacent free blocks
    coalesce();
}

// Example usage
int main() {
    int* arr = (int*)myMalloc(10 * sizeof(int));

    if (arr == nullptr) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    myFree(arr);

    return 0;
}
