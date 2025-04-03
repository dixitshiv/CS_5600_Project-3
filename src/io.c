#include "../include/io.h"
#include <stdlib.h>

// Initialize RNG
void os_srand(unsigned int seed)
{
    srand(seed);
}

// Simulate a random I/O request
int IO_request()
{
    return (rand() % CHANCE_OF_IO_REQUEST == 0); // ~10% chance
}

// Simulate random I/O completion
int IO_complete()
{
    return (rand() % CHANCE_OF_IO_COMPLETE == 0); // ~25% chance
}
