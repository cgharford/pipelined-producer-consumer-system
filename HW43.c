/*  Christina Harford
 *
 *  In accordance with the UNC Honor pledge, I certify that I have neither
 *  given nor recieved unauthorized aid on this assignment.
 *
 *  This program reads in characters from stdin, converts each adjacent pair of
 *  of asterisks to a carrot, and writes the character to stdout until an EOF
 *  character is read. This program is part three of four other programs that
 *  make up a producer/consumer system. When piped together in the linux shell,
 *  the four programs work together as a pipeline system, reading in lines of
 *  input from the keyboard, replacing every carriage return with a space,
 *  replacing every pair of asterisks (**) with a carrot (^), and finally
 *  printing out 80 character lines of output with a carriage return on the end.
 *  When the EOF character is read, the program terminates.
 */

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

// Reads in character input from stdin and writes it to stdout, converting each
// adjacent pair of asterisks to a carrot
int main (int argc, char ** argv) {
  // Static variable previous maintains the previous character that was read
  static char previous = '\0';
  char current;
  // Loop continues to execute until EOF is read
  while(1) {
    read(READ, &current, sizeof(char));
    if (current == EOF) {
      // Covers the case in which the 80th character before an EOF is an '*'
      if (previous == '*') {
        write(WRITE, &previous, sizeof(char));
      }
      break;
    }
    if (previous == '*') {
      // If both previous and current char are *, write a carrot to stdout
      if (current == '*') {
         current = '^';
         write(WRITE, &current, sizeof(char));
      }
      // Otherwise, write both previous and current to stdout
      else {
          write(WRITE, &previous, sizeof(char));
          write(WRITE, &current, sizeof(char));
      }
    }
    // If current is not an *, simply write to stdout
    else if (current != '*') {
        write(WRITE, &current, sizeof(char));
    }
    // Set previous for next loop iteration
    previous = current;
  }
  // Once EOF is reached, writes character to stdout and finishes
  write(WRITE, &current, sizeof(char));
  return 0;
}
