/**
 * Author: Micah Roening
 * Last updated: 4/28/23
 * 
 * Outputs the maximum ASCII character numeric value from every line
 * in a specified .txt file in order.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "pthread_scorecard_generator_v2.h"

#define MAX_SECTION_SIZE 1000

typedef struct thread_data {
    int thread_id;
    char** data;
    int num_lines;
    int line_num;
    char* results;
} thread_data_t;


int main (int argc, char *argv[])
{

    size_t len = 120;
    int line_num = 0;
    char result[MAX_SECTION_SIZE];
    char* data[MAX_SECTION_SIZE];

    printf("Started program...\n");

    if (argc < 2)
    {
        printf("ERROR: Parameters should be in the form of  %s <text file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char filename[MAX_FILE_NAME];
    strcpy(filename, argv[1]);

    FILE *serial_fd = fopen(filename, "r");
    FILE *fd = fopen(filename, "r");
    
    if (serial_fd == NULL || fd == NULL)
    {
        printf("ERROR: could not open file\n");
        return EXIT_FAILURE;
    }

    printf("Opened %s...\n", filename);

    // int num_lines = count_lines(serial_fd);
    int num_lines = 1000000;
    int num_sections = num_lines/MAX_SECTION_SIZE;
    int section_size = MAX_SECTION_SIZE;

    pthread_t threads[num_sections+1];

    int i = 0;
    for(i; i < num_sections + 1; i++)
    {
        line_num = i * MAX_SECTION_SIZE;
        if (i == num_sections)
        {
            section_size = num_lines % MAX_SECTION_SIZE;
        }

        get_lines(data, section_size, fd);

        thread_data_t * thread_args =  malloc(sizeof(*thread_args));

        thread_args->thread_id = i;
        thread_args->data = data;
        thread_args->num_lines = num_lines;
        thread_args->line_num = line_num;
        thread_args->results = result;

        printf("Creating thread #%d\n", i);

        int rc;
        rc = pthread_create(&threads[i], NULL, generate_scorecard_section, (void*)thread_args);
        if (rc)
        {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    
    printf("Created threads...\n");

    thread_data_t * return_data[num_sections+1];
   
    int j = 0;
    for(j; j < num_sections + 1; j++)
    {
        void** return_pointer = malloc(sizeof(void**));
        pthread_join(threads[j], return_pointer);
        return_data[j] = *(return_pointer);
    }

    printf("Joined threads...\n");
    printf("%s scorecard:\n", filename);

    int k = 0;
    for(k; k < num_sections + 1; k++)
    {
        print_scorecard_section(return_data[k]);
    }

    fclose(serial_fd);
    fclose(fd);

}

int count_lines(FILE *fd)
{
    char c;
    int count = 0;
    for (c; c != EOF; c = getc(fd))
        if (c == '\n') count++;

    return count;
}

void get_lines(char** data, int num_lines, FILE* fd)
{
    size_t len = 120;
  
    int i = 0;
    for(i; i < num_lines; i++)
    {
        char* new_line = (char*)malloc(len * sizeof(char));
        getline(&new_line, &len, fd);
        data[i] = new_line;
    }
}

void* generate_scorecard_section(void* thread_args)
{
    thread_data_t * args = (thread_data_t*)thread_args;

    size_t len = 120;
    char* line = (char*)malloc(len * sizeof(char));
    char c;
    char max_c = '0';
    
    int i = 0;
    for(i = 0; i < args->num_lines; i++)
    {
        int next = 0;      
        line = args->data[i];
        c = *line;
        max_c = c;
  
        while(c != '\n' && c != '\0')
        {
            c = *(line+next);

            if(c > max_c)
            {
                max_c = c;
            }
 
            next++;
        }

        args->results[i] = max_c;
    }

    return (void*) args;
}

void print_scorecard_section(thread_data_t* args)
{
    int i = 0;
    for(i; i < args->num_lines; i++)
    {
        printf("%d: %c\n", i + args->line_num, args->results[i]);
    }
}
