#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


#define ARR_SIZE 1000000
#define STR_SIZE 2001

float line_length[ARR_SIZE];
char char_array[ARR_SIZE][STR_SIZE];
float line_counts[ARR_SIZE];	// global count of individual characters
float local_line_count[ARR_SIZE];
float local_largest_count[ARR_SIZE];
int THREAD; 

void setup_array()
{
  int i;
  int j;
  int errorNum; 
  FILE *file;
  char *line = (char*) malloc( 2001 );
  for ( i = 0; i < ARR_SIZE; i++ ) {
      line_length[i] = 0.0;
  }
  //Open the file and store characters in the arrays
  file = fopen("/homes/dan/625/wiki_dump.txt", "r");
  for ( i = 0; i < ARR_SIZE; i++) {
    
	line_length[i] = (float)strlen(line);
	for ( j = 0; j < line_length[i]; j++ ) {
		 char_array[i][j] = line[j];
	}
    }
    for ( i = 0; i < ARR_SIZE; i++ ) {
  	line_counts[i] = 0;
	local_line_count[i] = 0;
	local_largest_count[i] = 0;
  } 
  //Close the file
  fclose(file);
}

void *count_array(void *rank)
{
  char theChar;
  int i;
  int j;
  int characterLocation;
  int myID =  *((int*) rank);
  int local_char_count[ARR_SIZE];
  int startingPosition = ((long) myID) * (ARR_SIZE / THREAD);
  int endingPosition = startingPosition + (ARR_SIZE / THREAD);


//Calculate the ASCII count in the array
  for ( i = startingPosition; i < endingPosition; i++) {
		local_char_count[i] = 0;
		for ( j = 0; j < line_length[i]; j++ ) {
			theChar = char_array[i][j];
			// if(local_largest_count[i] <= (int) theChar){
			// 	local_largest_count[i] = (int) theChar;
			// }
			
			local_char_count[i] += characterLocation;
		}
	}
	//I know this calculation is looking for average, but could not test for the right solution
	// Due to beocat queue being long
	 for ( i = startingPosition; i < endingPosition; i++ ) {
		 local_line_count[i] = local_char_count[i]/line_length[i];
	 }
}

void print_results()
{
  int i,j, total = 0;
  for ( i = 0; i < ARR_SIZE; i++ ) {
     printf("%d: %.1f\n", i, line_counts[i]);
  }
}

int main(int argc, char* argv[]) 
{
	int i, result;
	int numtasks, rank;
	MPI_Status Status;
	//Used to keep track of run time
	struct timeval time1, time2;
	double timeResult;
	
	gettimeofday(&time1, NULL);
	
	printf("DEBUG: starting loop on %s\n", getenv("HOSTNAME"));
	
	result = MPI_Init(&argc,&argv);
	//If MPI isnot successful, exist the program
	if (result != MPI_SUCCESS) {
	    printf ("errorNumor starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, result);
    }
	
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	THREAD = numtasks; 
	printf("size = %d rank = %d\n", numtasks, rank);
	fflush(stdout);

	if ( rank == 0 ) {
		//initialize the array
		init_arrays();
	}
	MPI_Bcast(char_array, ARR_SIZE * STR_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Bcast(line_length, ARR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	count_array(&rank);
	MPI_Reduce(local_line_count, line_counts, ARR_SIZE, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	if ( rank == 0 ) {
		print_results();
	}

	MPI_Finalize();
	gettimeofday(&time2, NULL);

	//Run time calculation
	timeResult = (time2.tv_sec - time1.tv_sec) * 1000.0; 
	timeResult += (time2.tv_usec - time1.tv_usec) / 1000.0;
	printf("DATA, %d, %s, %f\n", 1, getenv("SLURM_NTASKS"),  timeResult);
	return 0;
}