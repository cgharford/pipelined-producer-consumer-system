/*  Christina Harford
 *
 *  In accordance with the UNC Honor pledge, I certify that I have neither
 *  given nor recieved unauthorized aid on this assignment.
 *
 *  This program reads in characters from stdin, and prints the characters out
 *  in 80 character lines until an EOF character is read. This program is part
 *  four of four other programs that make up a producer/consumer system. When
 *  piped together in the linux shell, the four programs work together as a
 *  pipeline system, reading in lines of input from the keyboard, replacing
 *  every carriage return with a space, replacing every pair of asterisks (**)
 *  with a carrot (^), and finally printing out 80 character lines of output
 *  with a carriage return on the end. When the EOF character is read, the
 *  program terminates.
 */

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define OUTPUT_LENGTH 80
#define READ 0
#define WRITE 1

// Reads characters from stdin and prints as 80 character lines
int main (int argc, char ** argv) {
  char output[OUTPUT_LENGTH];
  memset(output, 0, sizeof(output));
  int i = 0;
  char current;
  // Loop continues to execute until EOF is read
  while(1) {
    read(READ, &current, sizeof(char));
    if (current == EOF) {
      break;
    }
    // Save characters to output in character array until we reach max
    output[i] = current;
    // i keeps track of how many output characters we have in our array
    i++;
    // Once we reach max of output characters, print out and reset values
    if (i == OUTPUT_LENGTH) {
      for (i = 0; i < OUTPUT_LENGTH; i++) {
        printf("%c", output[i]);
      }
      printf("\n");
      memset(output, 0, sizeof(output));
      i = 0;
    }
  }
  return 0;
}
