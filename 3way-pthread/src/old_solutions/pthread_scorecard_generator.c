/**
 * Author: Micah Roening
 * Last updated: 4/28/23
 *
 * Outputs the maximum ASCII character numeric value from every line
 * in a specified .txt file in order.
 *
 * TODO: review counting threads, maybe through slurm / cmd args
 *       review memory management, see /homes/dan/625
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "pthread_scorecard_generator.h"

#define NUM_THREADS 48
#define MAX_SECTION_SIZE 1000000

pthread_mutex_t loop_mutex;

typedef struct scorecard {
    // A pointer to an array of pointers
    // Those pointers each point to an array of characters
    // Which make up the scorecard
    char (*(*data)[NUM_THREADS])[MAX_SECTION_SIZE];
} scorecard_t;

typedef struct thread_data {
    int thread_id;
    char (*scorecard_section)[MAX_SECTION_SIZE];
    char *filename;
    int start_line;
    int section_size;   
} thread_data_t;

int main(int argc, char *argv[])
{
    printf("Started program!\n");

    // track runtime
    double TIMER;
    struct timeval t1, t2;

    gettimeofday(&t1, NULL);

    printf("Started TIMER...\n");

    // get the filename
    char *filename = get_filename(argc, argv);
    printf("Opening file: %s\n", filename);
    FILE *fd = fopen(filename, "r");
    if (fd == NULL)
    {
        printf("Error: Could not open file\n");
        return -1;
    }    

    printf("Opened file...\n");

    // count lines
    int total_lines = count_lines(fd);
    if (total_lines <= 0)
    {
       printf("Error: No lines in file\n");
       return -2;
    }

    printf("%d lines read from file\n", total_lines);   
 
    // count cores
    int core_count = NUM_THREADS;
    if (core_count <= 0)
    {
        printf("Error: Could not allocate cores\n");
        return -3;
    } 

    printf("Allocated cores...\n");

    // allocate scoreboard memory
    // TODO: Think about making limits based on available stack size. Might also do this w/ pthreads?
    int section_size = total_lines / core_count;
    int remainder_size = total_lines % core_count;

    scorecard_t * sc = malloc(sizeof(*sc));
    sc->data = malloc(sizeof(void*)*NUM_THREADS) ;
    
    int i = 0;
    for(i; i < NUM_THREADS; i++)
        (*sc->data)[i] = malloc(sizeof(MAX_SECTION_SIZE+1));

    printf("Allocated memory...\n");

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); 

    // create threads
    pthread_t threads[core_count];

    int j = 0;
    if (remainder_size != 0)
    {   
        // construct parameters
        thread_data_t * thread_args =  malloc(sizeof(*thread_args));
        
        thread_args->thread_id = j; 
        thread_args->scorecard_section = (*sc->data)[j];
        thread_args->filename = filename;
        thread_args->start_line = j;
        thread_args->section_size = section_size + remainder_size;
        
        int rc;
        rc = pthread_create(&threads[j], &attr, generate_scorecard_section, (void*)thread_args); 
        if (rc) 
        {
            printf("Error: return code from pthread_create() is %d\n", rc);
            exit(-4);
        }
        j++;
    }

    for(j; j < core_count; j++)
    {
        // construct parameters
        thread_data_t * thread_args = malloc(sizeof(*thread_args));
        
        thread_args->thread_id = j; 
        thread_args->scorecard_section = (*sc->data)[j];
        thread_args->filename = filename;
        if (remainder_size != 0) thread_args->start_line = (section_size * j) + remainder_size;
        else thread_args->start_line = section_size * j;
        thread_args->section_size = section_size;

        printf("Creating thread #%d\n", j);

        int rc;
        rc = pthread_create(&threads[j], &attr, generate_scorecard_section, (void*)thread_args); 
        if (rc) 
        {
            printf("Error: return code from pthread_create() is %d\n", rc);
            exit(-4);
        }
    }

    printf("Created threads...\n");

    // join threads
    pthread_attr_destroy(&attr);
    int k = 0;
    for(k; k < core_count; k++)
    {
        void** return_pointer = malloc(sizeof(void**));
        pthread_join(threads[k], return_pointer);
    }

    printf("Joined threads...\n");

    // print scoreboard
    print_scorecard(sc, core_count, section_size, remainder_size);       

    // free data
    // free_scorecard(sc, core_count);

    // close fd 
    fclose(fd);
 
    gettimeofday(&t2, NULL);

    TIMER = (t2.tv_sec - t1.tv_sec) * 1000.0; // seconds to millis
    TIMER += (t2.tv_usec = t1.tv_usec) / 1000.0; // micros to millis

    printf("\nSTATS:\n");
    printf("Total runtime: %f milliseconds\n", TIMER);
    printf("\n");
   
    return 0;
}

// TODO: Error checking, testing
char* get_filename(int argc, char *argv[])
{
    char* file_path = malloc(sizeof(char)*MAX_FILE_NAME);
    if (argc > 1)
    {
        strncpy(file_path, argv[1], (size_t)MAX_FILE_NAME);   
    }
    else
    {
        printf("(Press enter for default test file)\n");
        printf("Enter file path: ");
        fgets(file_path, MAX_FILE_NAME, stdin);

        if (strcmp(file_path, "\n") == 0)
           strncpy(file_path, "../pthread_test_dump.txt", (size_t)MAX_FILE_NAME);
           //fd = fopen("/homes/dan/625/wiki_dump.txt", "r");
    }

    //printf("Opening file: %s\n", file_path);
    //fd = fopen(file_path, "r");

    return file_path;
}

// TODO: Revamp test file with odd cases, test
int count_lines(FILE *fd)
{
    char c;
    int count = 0;
    for (c; c != EOF; c = getc(fd))
        if (c == '\n') count++;

    return count;
}

void* generate_scorecard_section(void* thread_args)
{
    thread_data_t * data = (thread_data_t*)thread_args;

    int cur_line = 0;
    int section_count = 0; 
    int line_max_char = -1;    

    FILE *fd = fopen(data->filename, "r");

    pthread_mutex_lock(&loop_mutex);

    printf("Starting thread #%d loop...\n", data->thread_id);

    char c;
    for (c; c != EOF; c = getc(fd))
    {

        printf("%c", c);

        if (c == '\n' || c == '\0')
        {
             cur_line++;
             line_max_char = -1;

             if (cur_line > data->start_line) section_count++;        
        }
        if (cur_line == data->start_line + data->section_size) break;
        if (cur_line >= data->start_line)
        {
            if ((int) c > line_max_char) 
            {
                (*data->scorecard_section)[section_count] = c;
                line_max_char = (int) c;
            }
             
        }
    }

    printf("\nEnding thread #%d loop...\n", data->thread_id);
    pthread_mutex_unlock(&loop_mutex);  

    fclose(fd);
 
    // just to return something, may replace later
    void* void_pointer = malloc(sizeof(0));
    return void_pointer;
}

void print_scorecard(scorecard_t *sc, int section_count, int section_size, int remainder_size)
{
    printf("Scorecard:\n");

    int section = 0;
    int cur_line = 0; 

    

    // Remainder handler
    if(remainder_size != 0) 
    {
        char (*cur_section)[MAX_SECTION_SIZE] = (*sc->data)[section];

        int cur_section_line = 0;
        for(cur_section_line; cur_section_line < section_size + remainder_size; cur_section_line++)
        {
             char cur_char = (*cur_section)[cur_section_line];
             
             int cur_char_value = (int) cur_char;

             //printf("%d: %d\n", cur_line, cur_char_value);
             printf("%d: %c\n", cur_line, cur_char);
             cur_line++;                  
        }

        section++; 
    } 

    // Regular section
    for(section; section < section_count; section++)
    {
         // printf("Section count: %d\n", section);
         char (*cur_section)[MAX_SECTION_SIZE] = (*sc->data)[section];

         int cur_section_line = 0;
         for(cur_section_line; cur_section_line < section_size; cur_section_line++)
         {
             char cur_char = (*cur_section)[cur_section_line];
             
             int cur_char_value = (int) cur_char;

             //printf("%d: %d\n", cur_line, cur_char_value);
             printf("%d: %c\n", cur_line, cur_char);
             cur_line++;                  
         }
    }

    printf("End scorecard.\n");
}

/*
void free_scorecard(scorecard_t *sc, int section_count)
{
    int i = 0;
    for(i; i < section_count; i++)
        free(*(sc->data)+i);

    free(sc->data);
    free(sc);
}
*/



























