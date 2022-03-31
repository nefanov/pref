// inspired from: https://gist.github.com/osimola/7917568
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

const size_t memsize = 128 * 1024 * 1024;
const size_t elems = memsize / sizeof(uint32_t);

void shuffle(uint32_t* data, size_t elems);
void time_linear(const uint32_t* indices, size_t elems);
void time_noprefetch(const uint32_t* data, size_t elems);
void time_prefetch(const uint32_t* data, size_t elems, int prefetch);

int main(void) {
    uint32_t* data = (uint32_t*)malloc(memsize);

    for (uint32_t i = 0; i < elems; i++) {
        data[i] = i; // Get page faults out of the way now
    }

    for (int i = 0; i < 5; i++) {
        time_linear(data, elems);
        time_noprefetch(data, elems);
        time_prefetch(data, elems, 1);
        time_prefetch(data, elems, 4);
        time_prefetch(data, elems, 16);
        time_prefetch(data, elems, 32);
        time_prefetch(data, elems, 128);
        time_prefetch(data, elems, 256);
    }

    return 0;
}

void time_linear(const uint32_t* data, size_t elems) {
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    uint32_t sum = 0; // Let's keep the optimizer from optimizing away memory access
    for (int i = 0; i < elems; i++) {
        sum += data[i];
    }
    
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    int64_t millis = (end_time.tv_sec - start_time.tv_sec) * 1000 + 
        (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
    
    printf("Linear: %" PRIi64 " ms (%" PRIu32 ")\n", millis, sum);
}


void time_noprefetch(const uint32_t* data, size_t elems) {
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    srand(42); // Fixed sequence

    uint32_t sum = 0; // Let's keep the optimizer from optimizing away memory access
    for (int i = 0; i < elems; i++) {
        sum += data[rand() % elems];
    }
    
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    int64_t millis = (end_time.tv_sec - start_time.tv_sec) * 1000 + 
        (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
    
    printf("No prefetch: %" PRIi64 " ms (%" PRIu32 ")\n", millis, sum);
}

void time_prefetch(const uint32_t* data, size_t elems, int prefetch) {
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    srand(42); // Fixed sequence
    uint32_t sum = 0; // Let's keep the optimizer from optimizing away memory access
    
    uint32_t buffer[prefetch];
    for (size_t i = 0; i < prefetch; i++)
        buffer[i] = rand() % elems;

    
    // Iteration countt to number of elems without resorting to modulo
    // prefetch or checking for two conditions inside inner
    // loops. Note that elems is a multiple of prefetch :)
    for (size_t i = 0; i < elems;) {
        for (size_t j = 0; j < prefetch; j++) {
            sum += data[buffer[j]];
            
            uint32_t next = rand() % elems;
            __builtin_prefetch(data + next);
            buffer[j] = next;
            i++;
        }
    }

    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    int64_t millis = (end_time.tv_sec - start_time.tv_sec) * 1000 + 
        (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
    
    printf("Prefetch %d: %" PRIi64 " ms (%" PRIu32 ")\n", prefetch, millis, sum);
}

void shuffle(uint32_t* data, size_t elems) {
    srand(42); // Fixed sequence

    for (size_t i = elems; i > 0; i--) {
        size_t target = rand() % i;
        uint32_t tmp = data[target];
        data[target] = data[i - 1];
        data[i - 1] = tmp;
    }
}
