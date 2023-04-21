#include <stdio.h>
#include "mpi.h"


#define ARR_SIZE 1000000;
#define STR_SIZE 2001;

char char_array[ARR_SIZE][STR_SIZE];
float line_length[ARR_SIZE];
float line_counts[ARR_SIZE];	// count of individual char (global)
float local_line_count[ARR_SIZE];
int THREADS;

void *calculate_array(void *rank){

    ID = *((int*) rank);
    int char_count[ARR_SIZE];
    int startingPosition = ((long) ID) * (ARR_SIZE / THREADS);
    int endPosition = startingPosition + (ARR_SIZE / THREADS);

    for (int i = startingPosition; i < endPosition; i++){
        char_count[i] = 0;
        for (int j = 0; j < line_length[i]; j++){
            char currentChar = char_array[i][j];
            int characterLocation = int(currentChar);

            char_count[i] += characterLocation
        }
    }
    for (int i = startingPosition; i < endPosition; i++){
        local_line_count[i] = local_line_count[i]/line_length[i];
    }
}

//Need to open the file
void array_initialize(){

    FILE *file;
    char *line = (char *) malloc(STR_SIZE);
    int error;

    //Initialize the line_length so its not empty
    for (int i = 0; i < ARR_SIZE; i++) {
        line_length[i] = 0.0;
        line_counts[i] = 0;
        local_line_count[i] = 0;
    }

    // for (int i = 0; i < ARR_SIZE; i++ ) {
    //     line_counts[i] = 0;
    //     local_line_count[i] = 0;
    // } 

    file = fopen("/homes/dan/625/wiki_dump.txt", "r");
    for ( i = 0; i < ARRAY_SIZE; i++) {
	    err = fscanf( file, "%[^\n]\n", line);
        if(err == EOF ) {
            break;
        }
        line_length[i] = (float)strlen(line);
        for ( j = 0; j < line_length[i]; j++ ) {
            char_array[i][j] = line[j];
        }
    }
    free(line);
    fclose(file);

}

void print(){
    for (int i = 0 ;i < ARR_SIZE; i++){
        printf("%d: %.1f\n", i, line_counts[i]);
    }
}



int main(int argc, char *argv[]){

    MPI_Status status;
    int num, rank, size, tag, next, from;

    int result = MPI_Init(&argc, &argv);
    if (result == MPI_SUCCESS){
        MPI_Comm_size(MPI_COMM_WORLD,&num);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    }
    MPI_ABORT(MPI_COMM_WORLD, result);
    


    MPI_Finalize();
    return 0;
}