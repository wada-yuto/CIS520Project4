/**
 * Author: Micah Roening
 * Last updated: 4/30/23
 *
 * Outputs the maximum ASCII character numerica value from every line
 * of a specified .txt file in order.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include "pthread_scorecard_generator_v3.h"

#define MAX_LINES 1000000
#define MAX_LINE_LENGTH 1000
#define MAX_THREADS 40

// Structs
typedef struct thread_data {
    int thread_id;
    int start_index;
    int end_index;
    char** lines;
    char* max_chars;
} thread_data_t;

typedef struct processMem {
        uint32_t virtualMem;
        uint32_t physicalMem;
} processMem_t;

// Memory function prototypes
int parseLine(char *line);
void GetProcessMemory(processMem_t* processMem);

// Global variables
int num_threads = 0;
pthread_t threads[MAX_THREADS+1];
thread_data_t thread_data[MAX_THREADS+1];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char *filename;

int main (int argc, char *argv[])
{
    // Start timing the program
    double TIMER;
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    // Start timing CPU time of the program
    clock_t start = clock();

    // Start tracking memory usage of the program
    processMem_t myMem;

    // check for correct number of arguments
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    // set filename
    filename = argv[1];
    
    // Open file
    FILE* fd = fopen(filename, "r");
    if (fd == NULL)
    {
       fprintf(stderr, "ERROR: Could not open input file.\n");
       exit(1);
    }

    // Store the file by lines into lines
    char** lines = malloc(sizeof(char) * MAX_LINES * MAX_LINE_LENGTH);
    size_t num_lines = 0;
    char* line = malloc(sizeof(char) * MAX_LINE_LENGTH);

    while(fgets(line, MAX_LINE_LENGTH, fd) != NULL)
    {
        // Remove trailing \n
        char* newline = strchr(line, '\n');
        if (newline != NULL)
        {
            *newline = '\0';
        }

        // Allocate memory and add line to the array
        char* new_line = malloc(sizeof(char) * (strlen(line) + 1));
        if (new_line == NULL)
        {
            fprintf(stderr, "ERROR: Memory allocation failed\n");
            size_t i = 0;
            for (i; i < num_lines; i++)
            {
                free(lines[i]);
            }
            free(lines);
            free(line);
            fclose(fd);
            exit(1);
        }

        strcpy(new_line, line);

        lines[num_lines] = new_line;
        num_lines++;
    }

    free(line);
    fclose(fd);

    // Set up character storage
    char max_chars[num_lines];
    int i;
    for(i = 0; i < num_lines; i++)
    {
        max_chars[i] = '\0';
    }

    // Create threads
    int lines_per_thread = num_lines / MAX_THREADS;
    for(num_threads; num_threads < MAX_THREADS+1; num_threads++)
    {
        thread_data[num_threads].thread_id = num_threads;
        thread_data[num_threads].start_index = num_threads * lines_per_thread;
        thread_data[num_threads].end_index = (num_threads + 1) * lines_per_thread;
        thread_data[num_threads].lines = lines;
        thread_data[num_threads].max_chars = max_chars;
        
        if (num_threads == MAX_THREADS)
        {
            thread_data[num_threads].end_index = num_lines;
        }

        pthread_create(&threads[num_threads], NULL, find_max_chars, (void*) &thread_data[num_threads]);
    }

    // Join threads
    for(i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print results
    for(i = 0; i < num_lines; i++)
    {
        printf("%d: %c\n", i, max_chars[i]);
    }  

    // Track memory
    GetProcessMemory(&myMem);

    // Free memory
    for (i = 0; i < num_lines; i++) 
    {
        free(lines[i]);
    }
    free(lines);

    // Calcuate stats
    gettimeofday(&t2, NULL);

    TIMER = (t2.tv_sec - t1.tv_sec) * 1000.0; // seconds to millis
    TIMER += (t2.tv_usec = t1.tv_usec) / 1000.0; // micros to millis

    clock_t end = clock();
    double cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000;

    printf("\nSTATS:\n");
    printf("Total runtime: %f milliseconds\n", TIMER);
    printf("CPU runtime: %f milliseconds\n", cpu_time_used);
    printf("Memory: vMem %u KB, pMem %u KB\n", myMem.virtualMem, myMem.physicalMem);
    printf("Total threads: %d\n", num_threads);
    printf("\n");

    return 0;
}

void *find_max_chars(void* args)
{
    pthread_mutex_lock(&mutex);
    thread_data_t *data = (thread_data_t*) args;

    int i;
    for (i = data->start_index; i < data->end_index; i++)
    {
        char max_char = '\0';
        char* line = data->lines[i];
  
        int j;
        for (j = 0; line[j] != '\0'; j++)
        {
            if(line[j] > max_char)
            {
                max_char = line[j];
            }
        }
        data->max_chars[i] = max_char;
    }

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

// Author: Dan Andresen
// See /homes/dan/625/checkmem.c
int parseLine(char *line) {
        int i = strlen(line);
        const char *p = line;
        while (*p < '0' || *p > '9') p++;
        line[i - 3] = '\0';
        i = atoi(p);
        return i;
}

// Author: Dan Andresen
// See /homes/dan/625/checkmem.c
void GetProcessMemory(processMem_t* processMem) {
        FILE *file = fopen("/proc/self/status", "r");
        char line[128];

        while (fgets(line, 128, file) != NULL) {
                if (strncmp(line, "VmSize:", 7) == 0) {
                        processMem->virtualMem = parseLine(line);
                }

                if (strncmp(line, "VmRSS:", 6) == 0) {
                        processMem->physicalMem = parseLine(line);
                }
        }

        fclose(file);
}


