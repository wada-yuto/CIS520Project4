#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OpenMP.h"
#include <sys/time.h>
#include <time.h>

#define N 100
#define NUM_LINES 1000000

int main(int argc, char *argv[]) {
    size_t len = 120;
    int line_num = 0;
    char result[N];
    char* data[N];
    struct timeval t1, t2;
    double elapsedTime;

    //get beginning time
    gettimeofday(&t1, NULL);

    //set number of threads
    omp_set_num_threads(20);

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
    //runs through the code in sections of N lines
    for(int j = 0; j < sections + 1; j++) {
        //set the starting line number of the section
        line_num = j * N;
        //find the remainder number of lines if it is under N
        if(j == sections) {
            lines = num_lines % N; 
        }
        //get the lines from the file
        get_lines(data, lines, file);
        //get the max char of each line
        process_lines(data, lines, line_num, result);
        //print the results
        print_results(result, lines, line_num);
        //free data from section
        for(int k = 0; k < lines; k++) {
            free(data[k]);
        }
    }

    //close the file
    fclose(file);

    //calculate run-time
    gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("Total elapsed time: %f\n", elapsedTime);
}

void get_lines(char** data, int num_lines, FILE* file) {
    size_t len = 120;
    //get lines from the file and put in the data aray
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
    //find the max character of each line in parallel
    #pragma omp parallel shared(len, results) private(line, c, max_c, i) 
    {
        #pragma omp for schedule(static) 
        for(i = 0; i < num_lines; i++) 
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
            results[i] = max_c; //place max character in array
        }
    }
}

void print_results(char* results, int num_lines, int line_num) {
     //print out the resulting max characters of each line
    for(int j = 0; j < num_lines; j++) 
    {
        printf("%d: %d\n", j, result[j]);
        //printf("%d: %c\n", j + line_num, results[j]);
    }
}