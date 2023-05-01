#ifndef MPI_H__
#define MPI_H__


#ifdef __cplusplus
extern "C"
{
#endif

    void setup_array();

    void *count_array(void *rank);

    void print_results();

    int main(int argc, char* argv[]) ;


#ifdef __cplusplus
}
#endif

#endif
