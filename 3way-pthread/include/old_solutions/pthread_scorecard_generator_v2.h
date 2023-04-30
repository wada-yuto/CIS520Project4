#ifndef PTHREAD_SCORECARD_GENERATOR_H__
#define PTHREAD_SCORECARD_GENERATOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_FILE_NAME 128

    typedef struct thread_data thread_data_t;

    //
    // Counts the number of lines within a provided file
    // \param fd File descriptor
    // \return The number of lines in the file, negative number on error
    //
    int count_lines(FILE *fd);
    
    //
    // Gets the lines from the file and stores them
    // \param data Stores the file information
    // \param num_lines The number of lines in the file
    // \param fd The file
    //
    void get_lines(char** data, int num_lines, FILE* fd);

    //
    // Generates the scoreboard for a specified section of the file
    // \param thread_args Parameters, see thread_data struct
    //
    void* generate_scorecard_section(void* thread_args); 

    //
    // Prints the completed scorecard
    // \param args A pointer with the data to be printed
    //
    void print_scorecard_section(thread_data_t* args);

#ifdef __cplusplus
}
#endif

#endif
