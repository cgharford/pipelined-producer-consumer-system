/*  Christina Harford
 *
 *  In accordance with the UNC Honor pledge, I certify that I have neither
 *  given nor recieved unauthorized aid on this assignment.
 *
 *  This program reads in character input and writes it to stdout until an EOF
 *  character is read. This program is part one of four other programs that make
 *  up a producer/consumer system. When piped together in the linux shell, the
 *  four programs work together as a pipeline system, reading in lines of input
 *  from the keyboard, replacing every carriage return with a space, replacing
 *  every pair of asterisks (**) with a carrot (^), and finally printing out 80
 *  character lines of output with a carriage return on the end.  When the EOF
 *  character is read, the program terminates.
 */

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

// Reads in character input from stdin and writes it to stdout
int main (int argc, char ** argv) {
  char current;
  // While loop continues to execute until EOF read
  while((current = getc(stdin)) != EOF) {
    write(WRITE, &current, sizeof(char));
  }
  // Once EOF is reached, writes EOF character to stdout and finishes
  write(WRITE, &current, sizeof(char));
  return 0;
}
