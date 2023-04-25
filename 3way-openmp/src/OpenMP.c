#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OpenMP.h"

#define N 100
#define NUM_LINES 1000000

int main(int argc, char *argv[]) {
    size_t len = 120;
    int line_num = 0;
    char result[N];
    char* data[N];

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

    int num_lines = NUM_LINES;
    int sections = num_lines/N;
    int lines = N;
    for(int j = 0; j < sections + 1; j++) {
        line_num = j * N;
        if(j == sections) {
            lines = num_lines % N;
        }
        get_lines(data, lines, file);
        //printf("get_lines\n");
        process_lines(data, lines, line_num, result);
        //printf("process_lines\n");
        print_results(result, lines, line_num);
        //printf("print_lines\n");
        //printf("section %d complete\n", j);
    }

    //close the file
    fclose(file);
}

void get_lines(char** data, int num_lines, FILE* file) {
    size_t len = 120;
    for(int j = 0; j < num_lines; j++) {
        char* new_line = (char*)malloc(len * sizeof(char));
        getline(&new_line, &len, file);
        data[j] = new_line;
    }

}

void process_lines(char** data, int num_lines, int line_num, char* results) {
    size_t len = 120;
    char* line = (char*)malloc(len * sizeof(char));
    char c;
    char max_c = '0';
    int i;
    //printf("start parallel\n");
    #pragma omp parallel shared(len, results) private(line, c, max_c, i) 
    {
        #pragma omp for schedule(static) 
        for(i = 0; i < num_lines; i++) 
        {
            //gets the max character from each line and puts them into an array
            int next = 0;
            //printf("assign line\n");
            line = data[i];
            //printf("%s\n", line);
            c = *line;
            //printf("assign max\n"); 
            max_c = c; 
            //printf("while loop\n");
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
            //printf("max: %c\n", max_c);
            results[i] = max_c; //place max character in array
        }
    }
}

void print_results(char* results, int num_lines, int line_num) {
     //print out the resulting max characters of each line
    for(int j = 0; j < num_lines; j++) 
    {
        //printf("%d: %d\n", j, result[j]);
        printf("%d: %c\n", j + line_num, results[j]);
    }
}