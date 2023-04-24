#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "MPI.h"
#include <mpi.h>

#define ARR_SIZE 1000000
#define STR_SIZE 2001

char char_array[ARR_SIZE][STR_SIZE];
float line_length[ARR_SIZE];
float line_counts[ARR_SIZE];	// count of individual char (global)
float local_line_count[ARR_SIZE];
int THREADS;

void *calculate_array(void *rank){

    int ID = *((int*) rank);
    int char_count[ARR_SIZE];
    int startingPosition = ((long) ID) * (ARR_SIZE / THREADS);
    int endPosition = startingPosition + (ARR_SIZE / THREADS);

    for (int i = startingPosition; i < endPosition; i++){
        char_count[i] = 0;
        for (int j = 0; j < line_length[i]; j++){
            char currentChar = char_array[i][j];
            int characterLocation = (int) currentChar;

            char_count[i] += characterLocation;
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
    for ( int i = 0; i < ARR_SIZE; i++) {
	    int err = fscanf( file, "%[^\n]\n", line);
        if(err == EOF ) {
            break;
        }
        line_length[i] = (float)strlen(line);
        for ( int j = 0; j < line_length[i]; j++ ) {
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
    double runTime;

    struct timeval time1;
    struct timeval time2;


    gettimeofday(&time1, NULL);

    int result = MPI_Init(&argc, &argv);
    if (result == MPI_SUCCESS){
        MPI_Comm_size(MPI_COMM_WORLD,&num);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);

        THREADS = num;

        if (rank == 0) array_initialize();
        
        MPI_Bcast(char_array, ARR_SIZE * STR_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
        MPI_Bcast(line_length, ARR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
        calculate_array(&rank);
        MPI_Reduce(local_line_count, line_counts, ARR_SIZE, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) print();
        MPI_Finalize();

        //Call function
        gettimeofday(&time2, NULL);
        runTime = (time2.tv_sec - time1.tv_sec) * 1000.0; //sec to ms
        runTime += (time2.tv_usec - time1.tv_usec) / 1000.0; // us to ms
        printf("DATA, %d, %s, %f\n", 1, getenv("SLURM_NTASKS"),  runTime);

        MPI_Finalize();
        return 0;
    }
    MPI_Abort(MPI_COMM_WORLD, result);
  
}