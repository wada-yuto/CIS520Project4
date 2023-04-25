#ifndef OPENMP_H__
#define OPENMP_H__

#ifdef __cplusplus
extern "C"
{
#endif

void get_lines(char** data, int num_lines, FILE* file);

void process_lines(char** data, int num_lines, int line_num, char* results);

void print_results(char* results, int num_lines, int line_num);

#ifdef __cplusplus
}
#endif

#endif