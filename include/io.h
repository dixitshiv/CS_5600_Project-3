#ifndef IO_H
#define IO_H

// Chance out of N for I/O request or completion
#define CHANCE_OF_IO_REQUEST 10
#define CHANCE_OF_IO_COMPLETE 4

// Random I/O simulation
void os_srand(unsigned int seed);
int IO_request();  // Returns 1 if job should request I/O
int IO_complete(); // Returns 1 if I/O is complete

#endif // IO_H