/*  Christina Harford
 *
 *  In accordance with the UNC Honor pledge, I certify that I have neither
 *  given nor recieved unauthorized aid on this assignment.
 *
 *  This program reads in lines of input from the keyboard, replaces every
 *  carriage return with a space, replaces every pair of asterisks (**) with a
 *  carrot (^), and prints out 80 character lines of output with a carriage
 *  return on the end.  When the EOF character is read, the program terminates.
 *  The program implements this functionality using processes and pipes in a
 *  message passing system. These processes work together a pipeline system,
 *  with each process processing the input in a different manner. There are
 *  four child processes and three pipes, and each process acts as a consumer
 *  for the previous process and a producer for the next process. When an EOF
 *  character is read, each process stores the EOF into the next pipe so that
 *  the final pipe will be able to terminate the process.
 */

// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

// One extra char on the end of each to be terminated by a null character
#define OUTPUT_LENGTH 80
#define READ 0
#define WRITE 1

// Functions for the execution of each process following fork().
void getInputCharacters(int descriptor);
void convertNewline(int descriptor1, int descriptor2);
void convertAsterisks(int descriptor1, int descriptor2);
void outputCharacters(int descriptor);

int main (int argc, char ** argv) {

  // Child procces ids and and pipes for communication between the processes
  pid_t childPid1;
  pid_t childPid2;
  pid_t childPid3;
  pid_t childPid4;
  int status;
  int inputPipe[2];
  int newlinePipe[2];
  int asterisksPipe[2];

  // Create pipes
  if (pipe(inputPipe) == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (pipe(newlinePipe) == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (pipe(asterisksPipe) == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  childPid1 = fork();

  // If error occured on 1st fork(), print error message and terminate program
  if (childPid1 == -1) {
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  // If we're in the 1st child process, read in input
  if (childPid1 == 0) {
    close(inputPipe[READ]);
    getInputCharacters(inputPipe[WRITE]);
  }
  // If we're in the 1st parent process, keep forking child processes
  else {
    childPid2 = fork();
    // If error occured on 2nd fork(), print error message and exit program
    if (childPid2 == -1) {
      printf("%s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
    // If we're in the 2nd child process, convert newline characters to spaces
    if (childPid2 == 0) {
      close(inputPipe[WRITE]);
      close(newlinePipe[READ]);
      convertNewline(inputPipe[READ], newlinePipe[WRITE]);
    }
    // If we're in the 2nd parent process, keep forking child processes
    else {
      childPid3 = fork();
      // If error occured on 3rd fork(), print error message and exit program
      if (childPid3 == -1) {
        printf("%s\n", strerror(errno));
        exit(EXIT_FAILURE);
      }
      // If we're in the 3rd child process, convert adjacent '*' to '^'
      if (childPid3 == 0) {
        close(newlinePipe[WRITE]);
        close(asterisksPipe[READ]);
        convertAsterisks(newlinePipe[READ], asterisksPipe[WRITE]);
      }
      // If we're in the 3rd parent process, fork a final process
      else {
        childPid4 = fork();
        // If error occured on 4th fork(), print error message and exit program
        if (childPid4 == -1) {
          printf("%s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }
        // If we're in the final child process, output in 80 character lines
        if (childPid4 == 0) {
          close(asterisksPipe[WRITE]);
          outputCharacters(asterisksPipe[READ]);
        }
        // If we're in the final parent process, wait for the child process
        else {
          if (wait(&status) == -1) {
            printf("%s\n", strerror(errno));
          }
        }
      }
    }
  }
  return 0;
}

// Obtains character input and puts it into first pipe
void getInputCharacters(int descriptor) {
    char current;
    // Reads input one character at a time and puts in pipe
    // While loop continues to execute until EOF read
    while((current = getc(stdin)) != EOF) {
      write(descriptor, &current, sizeof(char));
    }
    // Once EOF is reached, writes character into pipe
    write(descriptor, &current, sizeof(char));
}

// Converts newline characters from previous pipe to spaces
void convertNewline(int descriptor1, int descriptor2) {
    char current;
    // Obtains char from previous pipe one at a time
    // Loop continues to execute until EOF is read
    while(1) {
      read(descriptor1, &current, sizeof(char));
      if (current == EOF) {
        break;
      }
      // If charcter is a newline, put space into next pipe
      if (current == '\n') {
        current = ' ';
        write(descriptor2, &current, sizeof(char));
      }
      // Otherwise, simply put charcter into next pipe
      else {
        write(descriptor2, &current, sizeof(char));
      }
    }
    // Once EOF is reached, writes character into pipe
    write(descriptor2, &current, sizeof(char));
}

// Cconverts two adjacent asterisks from previous pipe to a carrot
void convertAsterisks(int descriptor1, int descriptor2) {
    // Static variable previous maintains the previous character that was read
    static char previous = '\0';
    char current;

    // Obtains char from previous pipe one at a time
    // Loop continues to execute until EOF is read
    while(1) {
      read(descriptor1, &current, sizeof(char));
      if (current == EOF) {
        // Covers the case in which the 80th character before an EOF is an '*'
        if (previous == '*') {
          write(descriptor2, &previous, sizeof(char));
        }
        break;
      }
      if (previous == '*') {
        // If both previous and current char are *, write a carrot in the pipe
        if (current == '*') {
           current = '^';
           write(descriptor2, &current, sizeof(char));
        }
        // Otherwise, write both previous and current into pipe
        else {
            write(descriptor2, &previous, sizeof(char));
            write(descriptor2, &current, sizeof(char));
        }
      }
      // If current is not an *, simply write into pipe
      else if (current != '*') {
          write(descriptor2, &current, sizeof(char));
      }
      // Set previous for next loop iteration
      previous = current;
    }
    write(descriptor2, &current, sizeof(char));
}

void outputCharacters(int descriptor) {
    char output[OUTPUT_LENGTH];
    memset(output, 0, sizeof(output));
    int i = 0;
    char current;
    // Obtains char from previous pipe one at a time
    // Loop continues to execute until EOF is read
    while(1) {
        read(descriptor, &current, sizeof(char));
        if (current == EOF) {
          break;
        }
        // Save characters to output in character array until we reach max
        output[i] = current;
        // i keeps track of how many output characters we have in our array
        i++;
        // Once we reach max of output characters, print out and reset values
        if (i == OUTPUT_LENGTH) {
            printf("%s\n", output);
            memset(output, 0, sizeof(output));
            i = 0;
        }
    }
    return;
}
