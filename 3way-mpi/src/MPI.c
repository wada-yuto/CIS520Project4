#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
int NUM_THREADS; 

#define ARRAY_SIZE 1000000
#define STRING_SIZE 2001

float line_length[ARRAY_SIZE];
char char_array[ARRAY_SIZE][STRING_SIZE];
float line_counts[ARRAY_SIZE];	// global count of individual characters
float local_line_count[ARRAY_SIZE];

void init_arrays()
{
  int i, j, err; 
  FILE *file;
  char *line = (char*) malloc( 2001 );
  for ( i = 0; i < ARRAY_SIZE; i++ ) {
      line_length[i] = 0.0;
  }
  file = fopen("/homes/dan/625/wiki_dump.txt", "r");
  for ( i = 0; i < ARRAY_SIZE; i++) {
	err = fscanf( file, "%[^\n]\n", line);
    if( err == EOF ) break;

    
	line_length[i] = (float)strlen(line);
	for ( j = 0; j < line_length[i]; j++ ) {
		 char_array[i][j] = line[j];
	}
  }
   for ( i = 0; i < ARRAY_SIZE; i++ ) {
  	line_counts[i] = 0;
	local_line_count[i] = 0;
  } 
  fclose(file);
}

void *count_array(void *rank)
{
  char theChar;
  int i, j, charLoc;
  int myID =  *((int*) rank);
  int local_char_count[ARRAY_SIZE];
  int startPos = ((long) myID) * (ARRAY_SIZE / NUM_THREADS);
  int endPos = startPos + (ARRAY_SIZE / NUM_THREADS);

  printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos); fflush(stdout);

  for ( i = startPos; i < endPos; i++) {
		local_char_count[i] = 0;
		for ( j = 0; j < line_length[i]; j++ ) {
			theChar = char_array[i][j];
			charLoc = (int) theChar;
			
			local_char_count[i] += charLoc;
		}
	}
	 for ( i = startPos; i < endPos; i++ ) {
		 local_line_count[i] = local_char_count[i]/line_length[i];
	 }
}

void print_results()
{
  int i,j, total = 0;
  for ( i = 0; i < ARRAY_SIZE; i++ ) {
     printf("%d: %.1f\n", i, line_counts[i]);
  }
}

int main(int argc, char* argv[]) 
{
	int i, rc;
	int numtasks, rank;
	MPI_Status Status;
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;
	
	gettimeofday(&t1, NULL);
	
	printf("DEBUG: starting loop on %s\n", getenv("HOSTNAME"));
	
	rc = MPI_Init(&argc,&argv);
	if (rc != MPI_SUCCESS) {
	    printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
	
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	NUM_THREADS = numtasks; 
	printf("size = %d rank = %d\n", numtasks, rank);
	fflush(stdout);

	if ( rank == 0 ) {
		init_arrays();
	}
	MPI_Bcast(char_array, ARRAY_SIZE * STRING_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Bcast(line_length, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	count_array(&rank);
	MPI_Reduce(local_line_count, line_counts, ARRAY_SIZE, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if ( rank == 0 ) {
		print_results();
	}

	MPI_Finalize();
	gettimeofday(&t2, NULL);

	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
	return 0;
}