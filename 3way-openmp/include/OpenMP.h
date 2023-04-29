#ifndef OPENMP_H__
#define OPENMP_H__

#ifdef __cplusplus
extern "C"
{
#endif

//
//Gets a section of the lines from the file and puts them in a data array
// \param data a char array to store the lines from the file
// \param num_lines the number of lines to get from the file
// \param file the file being read from
// 
void get_lines(char** data, int num_lines, FILE* file);

//
//Finds the maximum character of each line and puts them in a char array
// \param data a char array of the lines to process
// \param num_lines the number of lines being processed
// \param line_num the line number of the start of the section being processed
//  \param results the array to store the maximum characters
//
void process_lines(char** data, int num_lines, int line_num, char* results);

//
//Prints the results of the program
// \param results the array storing the maximum characters
// \param num_lines the number of lines to print
// \param line_num the line number of the start of the section being printed
// 
void print_results(char* results, int num_lines, int line_num);

#ifdef __cplusplus
}
#endif

#endif