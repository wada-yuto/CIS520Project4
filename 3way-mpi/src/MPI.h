#ifndef MPI_H__
#define MPI_H__


#ifdef __cplusplus
extern "C"
{
#endif


    void *calculate_array(void *rank);

    void array_initialize();

    void print();

    int main(int argc, char *argv[]);


#ifdef __cplusplus
}
#endif

#endif
