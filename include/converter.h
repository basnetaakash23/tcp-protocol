/*

  HELPER.H
  ========
  (c) Paul Griffiths, 1999
  Email: paulgriffiths@cwcom.net

  Interface to socket helper functions. 

  Many of these functions are adapted from, inspired by, or 
  otherwise shamelessly plagiarised from "Unix Network 
  Programming", W Richard Stevens (Prentice Hall).

*/


#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdint.h>
#include <stdio.h>


/*  Function declarations  */
void process_file(char* buffer, int length_of_file, char* target_file, char* convert_option);
int typezero_input(int pos, char* binary_buffer, FILE* pointer, char* convert_option);
void convert_to_typeOne(FILE* pointer,  uint8_t amount, uint16_t numbers[]);
int typefirst_input(int pos, char* binary_buffer, FILE *pointer, char* convert_option);
void convert_to_typeZero(FILE* pointer, uint8_t amount, short numbers[]);

#endif  /* CONVERTER_H */
