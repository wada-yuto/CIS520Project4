#ifndef PTHREAD_SCORECARD_GENERATOR_H__
#define PTHREAD_SCORECARD_GENERATOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct scorecard scorecard_t;
    typedef struct thread_data thread_data_t;

    //
    // Gets, validates, and returns the file to be 'scorecarded'
    // \param argc Number of command line arguments
    // \param argv Command line arguments
    // \return File descriptor, negative number on error
    //
    int get_file(int argc, char *argv[]);

    //
    // Counts the number of lines within a provided file
    // \param fd File descriptor
    // \return The number of lines in the file, negative number on error
    //
    int count_lines(int fd);

    //
    // TODO: Investigate *how* to do this
    // Counts the available cores for the program
    // \return Count of available cores
    //
    //int count_cores();
    
    //
    // Generates the scoreboard for a specified section of the file
    // \param thread_args Parameters, see pthread_data struct
    //
    void* generate_scorecard_section(void* thread_args); 

    //
    // Prints the completed scorecard
    // \param sc A pointer to the scorecard to be printed
    //
    void print_scorecard(scorecard_t *sc, int section_count, int section_size, int remainder_size);

    //
    // Frees the memory of a scorecard object
    // \param sc The scorecard to be freed
    //
    void free_scorecard(scorecard_t *sc, int section_count);

#ifdef __cplusplus
}
#endif

#endif
