/**
 * Author: Micah Roening
 * Last updated: 4/16/23
 *
 * Outputs the maximum ASCII character numeric value from every line
 * in a specified .txt file in order.
 *
 * TODO: implement main
 *       discover how to find core count
 *       implement int get_file()
 *       implement int count_lines()
 *       implement void* generate_scoreboard_section();
 *       implement void print_scoreboard();
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pthread_scorecard_generator.h"

typedef struct scorecard {
    char** data;
} scorecard_t;

typedef struct thread_data {
    int thread_id;
    char *scorecard_section;
    int fd;
    int start_line;
    int section_size;   
} thread_data_t;

int main(int argc, char *argv[])
{
    printf("Hello World!\n");

    // get the file
    int fd = get_file(argc, argv);
    if (fd < 0)
    {
        printf("Error: Could not open file\n");
        return -1;
    }    

    // count lines
    int total_lines = count_lines(fd);
    if (total_lines <= 0)
    {
       printf("Error: No lines in file\n");
       return -2;
    }
    
    // count cores
    int core_count = 1; // TODO: Implement counting cores
    if (core_count <= 0)
    {
        printf("Error: Could not allocate cores\n");
        return -3;
    } 

    // allocate scoreboard memory
    // TODO: Think about making limits based on available stack size. Might also do this w/ pthreads?
    int section_size = total_lines / core_count;
    int remainder_size = total_lines % core_count;

    scorecard_t * sc = malloc(sizeof(scorecard_t));
    sc->data = malloc(sizeof(char*) * core_count);

    int i = 0;
    if (remainder_size != 0) // if there is an uneven block, it goes first
    {
        *(sc->data) = calloc((size_t)remainder_size, sizeof(char));
        i++;
    }

    for(i; i < core_count; i++)
    {
        char* scorecard_section = *(sc->data)+i;
        scorecard_section = calloc((size_t)section_size, sizeof(char));
    } 

    // create threads
    pthread_t threads[core_count];

    int j = 0;
    for(j; j < core_count; j++)
    {
        // construct parameters
        thread_data_t * thread_args;
        thread_args = malloc(sizeof(thread_data_t));
        
        thread_args->thread_id = j; 
        char* scorecard_section = *(sc->data)+j;
        thread_args->scorecard_section = scorecard_section;
        thread_args->fd = fd;
        thread_args->start_line = section_size * j;
        thread_args->section_size = section_size;

        int rc;
        rc = pthread_create(&threads[j], NULL, generate_scorecard_section, (void*)thread_args); 
        if (rc) 
        {
            printf("Error: return code from pthread_create() is %d\n", rc);
            exit(-4);
        }
    }

    // join threads

    // print scoreboard
    
    // free data

    return 0;
}

int get_file(int argc, char *argv[])
{
    return 0;
}

int count_lines(int fd)
{

    return 0;
}

void* generate_scorecard_section(void* thread_args)
{
    char* return_pointer = malloc(sizeof(char));
    return return_pointer;
}

void print_scorecard(scorecard_t *sc)
{
}

void free_scorecard(scorecard_t *sc)
{
}




























