//#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100

int main(int argc, char *argv[]) {
    //char a = 'a';
    //printf("%d", a);

    char* line, results;
    size_t* len;
    ssize_t bytes_read;
    size_t line_num = 0;
    char result[N];
    char c;
    char max_c = '0';
    int count = 0;

    *len = sizeof(char);

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

    for (c = getc(file); c != EOF; c = getc(file)) {
        if(c > max_c) {
            max_c = c;
        }
        if (c == '\n') {
            result[line_num] = max_c;
            max_c = '0';
            line_num++;
        }
    }

    for(int i = 0; i < N; i++) {
        printf("%d: %d\n", i, result[i]);
    }

    //close the file
    fclose(file);
}