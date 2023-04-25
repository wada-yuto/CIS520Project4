#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "OpenMP.h"

#define N 100
#define NUM_LINES 5

int main(int argc, char *argv[]) {
    // char a = 'a';
    // printf("%d", a);

    size_t len = 120;
    char* line = (char*)malloc(len * sizeof(char));
    //ssize_t bytes_read;
    int line_num = 0;
    char result[NUM_LINES];
    char* data[NUM_LINES];
    char c;
    char max_c = '0';
    //int count = 0;
    int i;

    //checks that enough arguments were input
    if (argc < 2) 
    {
        printf("PARAMETER FAILURE: %s <text file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    //get filename from arguments
    char filename[20];
    strcpy(filename, argv[1]);

    FILE* file = fopen(filename, "r"); //opens a read only file
    //checks if the file failed to open
    if(file == NULL) {
        printf("PARAMETER FAILURE: invalid filename\n");
        return EXIT_FAILURE;
    }

    for(int j = 0; j < NUM_LINES; j++) {
        char* new_line = (char*)malloc(len * sizeof(char));
        getline(&new_line, &len, file);
        data[j] = new_line;
    }

    // int sections = num_lines/N;
    // int lines = N;
    // for(int j = 0; j < sections + 1; j++) {
    //     if(j == sections) {
    //         lines = num_lines % N;
    //     }
    //     get_lines(data, lines, line_num);
    //     process_lines(data, lines, line_num, result);
    //     print_results(result, lines, line_num);
    // }

    // for(bytes_read = getline(&line, len, file); bytes_read != -1; bytes_read = getline(&line, len, file)) {
    //     printf("line %zu\n", line_num);
    //     size_t line_len = strlen(line);
    //     char max = *line;
    //     for(size_t i = 0; i < line_len; i++) {
    //         printf("character %zu\n", i);
    //         if(*(line+i) > max) {
    //             max = *(line+i);
    //         }
    //     }
    //     result[line_num] = max;
    //     line_num++;
    // }

    // for (c = getc(file); c != EOF; c = getc(file)) {
    //     //printf("char: %c\n", c);
    //     if(c > max_c) {
    //         max_c = c;
    //     }
    //     if (c == '\n') {
    //         printf("max char: %c", max_c);
    //         result[line_num] = max_c;
    //         max_c = '0';
    //         line_num++;
    //     }
    // }

    #pragma omp parallel shared(len, result) private(line, c, max_c, i) 
    {
        #pragma omp for schedule(static) 
        for(i = 0; i < NUM_LINES; i++) 
        {
            //gets the max character from each line and puts them into an array
            int next = 0;
            line = data[i];
            c = *line; 
            max_c = c; 
            //find max character from the line
            while(c != '\n' && c != '\0') 
            {
                c = *(line+next);
                //check if character is greater than the current max
                if(c > max_c) {
                    max_c = c;
                }
                next++;
            }
            result[i] = max_c; //place max character in array
        }
    }


    //print out the resulting max characters of each line
    for(int j = 0; j < NUM_LINES; j++) 
    {
        //printf("%d: %d\n", j, result[j]);
        printf("%d: %c\n", j, result[j]);
    }


    //close the file
    fclose(file);
}