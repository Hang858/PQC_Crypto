#include "malloc.h"
#include <stdint.h>
#include <stdio.h>

// Alignment macros
#define ALIGN_UP_4(size) (((size) + 3) & ~3)  // Align size up to 4-byte boundary

typedef struct {
    size_t size;        // Block size (excluding metadata)
    uint32_t is_free;        // 1 means free, 0 means allocated
} BlockMeta;

// Global heap memory and metadata (aligned to 4-byte boundary)
static uint32_t heap[HEAP_SIZE / 4];
static BlockMeta* heap_start = NULL;
static BlockMeta* last_free_block = NULL;  // Cache the most recent free block
static uint8_t initialized = 0;

/**
 * Initialize heap memory
 */
static void init_heap(void) {
    if (initialized) return;
    
    heap_start = (BlockMeta*)heap;
    heap_start->size = HEAP_SIZE - sizeof(BlockMeta);
    heap_start->is_free = 1;  // Initially the entire heap is free
    
    last_free_block = heap_start;  // Initialize last_free_block
    initialized = 1;
}

/**
 * Get the data pointer from a BlockMeta pointer
 */
static void* get_data_ptr(BlockMeta* meta) {
    return (void*)(meta + 1);  // Data immediately follows metadata
}

/**
 * Get the BlockMeta pointer from a data pointer
 */
static BlockMeta* get_meta_ptr(void* ptr) {
    if (ptr == NULL) return NULL;
    return ((BlockMeta*)ptr) - 1;
}

/**
 * Update max heap usage statistics (only for ENABLE_HEAP_TEST)
 */
#if ENABLE_HEAP_TEST
// Metadata structure for a memory block
static size_t max_heap_usage = 0;
static void update_max_heap_usage(void) {
    size_t current_allocated = 0;
    BlockMeta* curr = heap_start;
    uint8_t* heap_end_local = (uint8_t*)heap + HEAP_SIZE;
    
    while ((uint8_t*)curr < heap_end_local) {
        if (!curr->is_free) {
            current_allocated += curr->size;
        }
        uint8_t* next_ptr = (uint8_t*)curr + sizeof(BlockMeta) + curr->size;
        if (next_ptr >= heap_end_local) break;
        curr = (BlockMeta*)next_ptr;
    }
    
    if (current_allocated > max_heap_usage) {
        max_heap_usage = current_allocated;
    }
}
#else
#define update_max_heap_usage() ((void)0)
#endif

/**
 * Coalesce adjacent free blocks
 */
static void coalesce_free_blocks(void) {
    if (!initialized || heap_start == NULL) return;
    
    BlockMeta* current = heap_start;
    uint8_t* heap_end = (uint8_t*)heap + HEAP_SIZE;
    
    while ((uint8_t*)current < heap_end) {
        // Calculate the position of the next block
        uint8_t* next_block_ptr = (uint8_t*)current + sizeof(BlockMeta) + current->size;
        
        // Check for overflow
        if (next_block_ptr >= heap_end) break;
        
        BlockMeta* next = (BlockMeta*)next_block_ptr;
        
        // If current and next blocks are both free, merge them
        if (current->is_free && next->is_free) {
            current->size += sizeof(BlockMeta) + next->size;
            // Continue checking from the current block in case there are more merges
            continue;
        }
        
        current = next;
    }
}

/**
 * Custom malloc implementation
 */
void* my_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Align requested size to 4-byte boundary
    size_t aligned_size = ALIGN_UP_4(size);
    
    if (!initialized) {
        init_heap();
    }
    
    uint8_t* heap_end = (uint8_t*)heap + HEAP_SIZE;
    BlockMeta* current = heap_start;
    
    // Traverse all blocks to find a large enough free block
    while ((uint8_t*)current < heap_end) {
        if (current->is_free && current->size >= aligned_size) {
            // If the current block is much larger than requested, split it
            if (current->size > aligned_size + sizeof(BlockMeta)) {
                // Create a new block
                BlockMeta* new_block = (BlockMeta*)((uint8_t*)current + sizeof(BlockMeta) + aligned_size);
                new_block->size = current->size - aligned_size - sizeof(BlockMeta);
                new_block->is_free = 1;
                
                // Update current block
                current->size = aligned_size;
            }
            
            current->is_free = 0;  // Mark as allocated
            
            // Update last_free_block to the next block after this one
            uint8_t* next_block_ptr = (uint8_t*)current + sizeof(BlockMeta) + current->size;
            if (next_block_ptr < heap_end) {
                last_free_block = (BlockMeta*)next_block_ptr;
            } else {
                last_free_block = heap_start;
            }
            
            // Update heap usage statistics
            update_max_heap_usage();
            
            return get_data_ptr(current);
        }
        
        // Move to the next block
        uint8_t* next_block_ptr = (uint8_t*)current + sizeof(BlockMeta) + current->size;
        if (next_block_ptr >= heap_end) break;
        
        current = (BlockMeta*)next_block_ptr;
    }
    
    return NULL;
}

void* my_calloc(size_t nmemb, size_t size) {
    size_t total_size = size * nmemb;
    void* ptr = my_malloc(total_size);
    if (ptr) {
        // Since ptr is 4-byte aligned and total_size is multiple of 4,
        // use uint32_t to initialize memory for better performance
        uint32_t* p = (uint32_t*)ptr;
        size_t count = ALIGN_UP_4(total_size) / sizeof(uint32_t);
        for (size_t i = 0; i < count; i++) {
            p[i] = 0;
        }
    }
    return ptr;
}

/**
 * Custom free implementation
 */
void my_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    
    if (!initialized) {
        return;
    }
    
    BlockMeta* meta = get_meta_ptr(ptr);
    
    // Verify the pointer is within heap range
    if ((uint8_t*)meta < (uint8_t*)heap || (uint8_t*)meta >= (uint8_t*)heap + HEAP_SIZE) {
        return;
    }
    
    if (meta->is_free) {
        return;
    }
    
    meta->is_free = 1;  // Mark as free
    
    // Attempt to coalesce adjacent free blocks
    coalesce_free_blocks();
    
    // Update last_free_block to the meta block (after coalescing, it should still be valid)
    last_free_block = meta;
}

#if ENABLE_HEAP_TEST
typedef struct {
    size_t free_mem;
    size_t allocated_mem;
    uint32_t block_count;
} HeapStats;

static HeapStats collect_heap_stats(void) {
    HeapStats stats = {0, 0, 0};
    if (!initialized || heap_start == NULL) return stats;

    BlockMeta* current = heap_start;
    uint8_t* heap_end = (uint8_t*)heap + HEAP_SIZE;

    while ((uint8_t*)current < heap_end) {
        stats.block_count++;
        if (current->is_free) {
            stats.free_mem += current->size;
        } else {
            stats.allocated_mem += current->size;
        }

        uint8_t* next_block_ptr = (uint8_t*)current + sizeof(BlockMeta) + current->size;
        if (next_block_ptr >= heap_end) break;
        current = (BlockMeta*)next_block_ptr;
    }

    return stats;
}

size_t get_total_free_memory(void) {
    return collect_heap_stats().free_mem;
}

size_t get_total_allocated_memory(void) {
    return collect_heap_stats().allocated_mem;
}

uint32_t get_total_block_count(void) {
    return collect_heap_stats().block_count;
}

void reset_max_heap_usage(void) {
    max_heap_usage = 0;
}

size_t get_max_heap_usage(void) {
    return max_heap_usage;
}
#endif
