#ifndef PTHREAD_SCORECARD_GENERATOR_H__
#define PTHREAD_SCORECARD_GENERATOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct thread_data thread_data_t;
    typedef struct processMem processMem_t;

    //
    // Generates the scoreboard for a specified section of the file
    // \param args Parameters, see pthread_data struct
    //
    void* find_max_chars(void* args); 
   
    void GetProcessMemeory(processMem_t* processMem);

#ifdef __cplusplus
}
#endif

#endif
