#include <stdio.h> // For printf, etc.
#include <stdlib.h> // For atoi, etc.
#include <string.h> // Needed for various string manipulations.
#include <rpc/rpc.h> // Header for RPC calls.
#include "ssnfs.h" // Header for the RPC service created by rpcgen.
#include <readline/readline.h> // Readline is required for the main interactive loop.
#include <readline/history.h> // Needed for prompt history
#include <pwd.h> // Need to get username.

/*int main(int argc, char **argv)
{
    CLIENT *cl; // Pointer to the client handle.

    if (argc != 2)
    { // If the user doesn't enter the correct number of parameters...
        fprintf(stderr, "usage: %s <hostname>\n", argv[0]); // Print out usage information.
        exit(1); // And exit with an error code.
    }

    cl = clnt_create(argv[1], SSNFSPROG, SSNFSVER, "tcp"); // Create the RPC client.
    if (cl == NULL)
    { // If there was an error creating the client...
        fprintf(stderr, "getting client handle failed"); // Report an error message.
        exit(2); // And exit with an error code.
    }

    char * username = getpwuid(getuid())->pw_name; // Get the user's username.

    // Main Interactive Loop.
    char *linebuffer, *command, *filename, *tofilename;  // Strings for the line buffer, command, filename...
    int valid_command = 1; // Flag to determine if the user entered a valid command.
    // void *outp; // All the output structs have the same structure, just use a single pointer.
    // write_output *outp;
    char *offset, *numbytes, *buffer; // Parameters to read and write.
    for (;;) { // Loop until user enters EOF.
        linebuffer = readline("> "); // Prompt the user for a command.
        if (!linebuffer) // If the user enters and EOF...
            break; // exit the interactive loop with no error.
        add_history(linebuffer); // Add input to history.
        command = strtok(linebuffer, " "); // Split off the first word, the command.
        if (command)
        { // The line isn't blank...
            if (strcmp(command, "open") == 0)
            { // If the user enters the "create" command...
                open_input in; // Input struct for the RPC call.
                filename = strtok(NULL, " "); // Split off the name of the file to be created.
                if (filename)
                { // If the user does enter a filename...
                    strcpy(in.user_name, username); // Copy user name.
                    strcpy(in.file_name, filename);
                    open_output *outp = open_file_1(&in, cl); // And call the create file RPC.
                    if (outp == NULL)
                    { // If the RPC call fails...
                      valid_command = 0; // User entered a valid command.
                    }
                    printf("%i \t %s\n", outp->fd, in.file_name);
                }
                else
                { // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
                }
            }
            else if (strcmp(command, "read") == 0)
            { // If the user enters the "read" command...
                read_input in; // Input struct for the RPC call.
                char *read_fd = strtok(NULL, " "); // read the input file descriptor, split off the offset.
                if (read_fd)
                { // If the user does enter a file descriptor...
                  numbytes = strtok(NULL, " "); // read the number of bytes, split off the offset.
                  if (numbytes)
                  {
                      strcpy(in.user_name, username); // Copy user name.
                      in.fd = atoi(read_fd); // get the file descriptor and store inside of the input structure
                      in.numbytes = atoi(numbytes); // get the number of bytes and store inside of the input structure
                      read_output *outp = read_file_1(&in, cl); // And call the read file RPC.
                      if (outp == NULL)
                      { // If the RPC call fails...
                          valid_command = 1; // User entered a valid command.
                          printf("read failed.\n");
                          exit(3);
                      }
                      else{
                        printf("%s\n", outp->out_msg.out_msg_val);
                      }
                  }
                  else
                  {
                      printf("number of bytes to read required.\n"); // Report an error message
                  }
                }
                else
                { // If the user doesn't enter a filename...
                      printf("file descriptor required.\n"); // Report an error message.

                }
            }
            else if (strcmp(command, "write") == 0)
            { // If the user enters the "write" command...
                write_input in; // Input struct for the RPC call.
                int write_fd;
                strcpy(in.user_name, username); // Copy user name.
                write_fd = atoi(strtok(NULL, " ")); // Split off the file descriptor to which we will write.
                    if (write_fd)
                    { // If the user does enter a filename...
                        numbytes = strtok(NULL, " "); // Split off the length of the write.
                        if (numbytes)
                        { // If the user does enter an offset...
                            buffer = strtok(NULL, " "); // Split off the length of the write.
                            if (buffer)
                            {
                                strcpy(in.user_name, username); // Copy user name.
                                in.fd = write_fd;
                                in.numbytes = atoi(numbytes); // The number of bytes to write.
                                in.buffer.buffer_val = strdup(buffer);
                                in.buffer.buffer_len = strlen(buffer);
                                write_output *outp = write_file_1(&in, cl); // And call the create file RPC.
                                if (outp == NULL)
                                { // If the RPC call fails...
                                  valid_command = 0; // User entered a valid command.

                                }
                                else{
                                  printf("%s\n", outp->out_msg.out_msg_val);
                                }
                            }
                            else
                            {
                                printf("Data to write required.\n"); // Report an error message.
                            }
                        }
                        else
                        { // User didn't enter numbytes...
                            printf("Number of bytes required.\n"); // Report an error message.
                        }
                    }
                    else
                    { // User didn't enter an offset...
                        printf("file descriptor required.\n"); // Report an error message.
                    }

            }
            else if (strcmp(command, "list") == 0)
            { // If the user enters the "list" command...
                list_input in; // Input struct for the RPC call.
                strcpy(in.user_name, username); // Copy user name.
                list_output *outp = list_files_1(&in, cl); // And call the list file RPC.
                if (outp == NULL)
                { // If the RPC call fails...
                  valid_command = 0; // User entered a valid command.
                }
                else{
                  printf("%s\n", outp->out_msg.out_msg_val);
                }
            }
            else if (strcmp(command, "delete") == 0)
            { // If the user enters the "delete" command...
                delete_input in; // Input struct for the RPC call.
                filename = strtok(NULL, " "); // Split off the name of the file to be deleted.
                if (filename)
                { // If the user does enter a filename...
                    strcpy(in.user_name, username); // Copy user name.
                    strcpy(in.file_name, filename); // Copy file name.
                    delete_output *outp = delete_file_1(&in, cl); // And call the delete file RPC.
                    if (outp == NULL)
                    { // If the RPC call fails...
                      valid_command = 0; // User entered a valid command.
                    }
                }
                else
                { // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
                }
            }
            else if (strcmp(command, "close") == 0)
            {
                close_input in;
                filename = strtok(NULL, " "); // Split off the name of the file to be deleted.
                if (filename)
                { // If the user does enter a filename...
                    strcpy(in.user_name, username); // Copy user name.
                    close_output *outp = close_file_1(&in, cl); // And call the close file RPC.
                    if (outp == NULL)
                    { // If the RPC call fails...
                      valid_command = 0; // User entered an ivalid command.
                    }
                }
            }
            else
            { // User did not enter a recognized command.
                printf("Unrecognized command.\n");
            } // End of command identification branch.
            if(!valid_command)
            {
              fprintf(stderr, "remote procedure call failed\n"); // Report an error message.
              exit(3); // And exit with an error code.
            }
      }
      valid_command = 1; // Reset flag.
      free(linebuffer); // Free the buffer allocated by readline.
    }
    printf("\n"); // Make terminal look nice.
    exit(0); // End the program with no error.
}
*/
// CLIENT *cl; // Pointer to the client handle.


int Open(char *filename, CLIENT *cl)
{
  char * username = getpwuid(getuid())->pw_name; // Get the user's username.
  open_input in; // Input struct for the RPC call.
  strcpy(in.user_name, username); // Copy user name.
  strcpy(in.file_name, filename);
  open_output *outp = open_file_1(&in, cl); // And call the create file RPC.      // if (outp == NULL)

  if (outp == NULL)
  {
    printf("open unsuccessful. \n");
  }
  return outp->fd;
}



void Read(int fd, char *buffer, int numbytes, CLIENT *cl)
{
  char * username = getpwuid(getuid())->pw_name; // Get the user's username.
  read_input in;
  strcpy(in.user_name, username); // Copy user name.
  in.fd = fd; // get the file descriptor and store inside of the input structure
  in.numbytes = numbytes; // get the number of bytes and store inside of the input structure
  read_output *outp = read_file_1(&in, cl); // And call the read file RPC.
  strcpy(buffer,outp->out_msg.out_msg_val); // deep copy the read result to the buffer
  if (outp == NULL)
  {
    printf("read unsuccessful. \n");
  }
  else
  {
    printf("%s\n", outp->out_msg.out_msg_val);
  }
}

void Write(int fd, char *buffer, int numbytes, CLIENT *cl)
{
  char * username = getpwuid(getuid())->pw_name; // Get the user's username.
  write_input in;
  strcpy(in.user_name, username); // Copy user name.
  in.fd = fd;
  in.numbytes = numbytes; // The number of bytes to write.
  in.buffer.buffer_val = strdup(buffer);
  in.buffer.buffer_len = strlen(buffer);
  write_output *outp = write_file_1(&in, cl); // And call the create file RPC.
  if (outp == NULL)
  {
    printf("write unsuccessful.\n");
  }
  else
  {
    printf("%s\n", outp->out_msg.out_msg_val);
  }
}

void Close(int fd, CLIENT *cl)
{
  char * username = getpwuid(getuid())->pw_name; // Get the user's username.
  close_input in;
  in.fd = fd;
  strcpy(in.user_name, username); // Copy user name.
  close_output *outp = close_file_1(&in, cl); // And call the close file RPC.
  if (outp == NULL)
  {
    printf("close unsuccessful.\n");
  }
  else
  {
    printf("%s\n", outp->out_msg.out_msg_val);
  }
}

void Delete(char *filename, CLIENT *cl)
{
  char * username = getpwuid(getuid())->pw_name; // Get the user's username.
  delete_input in;
  strcpy(in.user_name, username); // Copy user name.
  strcpy(in.file_name, filename); // Copy file name.
  delete_output *outp = delete_file_1(&in, cl); // And call the delete file RPC.
  if (outp == NULL)
  {
    printf("delete unsuccessful. \n");
  }
  else
  {
    printf("%s\n", outp->out_msg.out_msg_val);
  }
}

void List(CLIENT *cl)
{
  char * username = getpwuid(getuid())->pw_name; // Get the user's username.
  list_input in; // Input struct for the RPC call.
  strcpy(in.user_name, username); // Copy user name.
  list_output *outp = list_files_1(&in, cl); // And call the list file RPC.
  if (outp == NULL)
  { // If the RPC call fails...
    printf("list unsuccessful. \n" );
  }
  else
  {
    printf("%s\n", outp->out_msg.out_msg_val);
  }
}


void main(int argc, char **argv)
{
  if (argc != 2)
  { // If the user doesn't enter the correct number of parameters...
      fprintf(stderr, "usage: %s <hostname>\n", argv[0]); // Print out usage information.
      exit(1); // And exit with an error code.
  }
  CLIENT *cl; // Pointer to the client handle.
  cl = clnt_create(argv[1], SSNFSPROG, SSNFSVER, "tcp"); // Create the RPC client.
  if (cl == NULL)
  { // If there was an error creating the client...
      fprintf(stderr, "getting client handle failed"); // Report an error message.
      exit(2); // And exit with an error code.
  }



  int i,j,k;
  int fd1,fd2,fd3;
  char buffer[100];
  fd1=Open("File1", cl); // opens the file
  for (i=0; i< 20;i++){
  Write(fd1, "funny contents in the file 1", 15, cl);
  }
  Close(fd1, cl);

  fd2=Open("File2", cl);
  for (j=0; j< 20;j++){
  Read(fd2, buffer, 10, cl);
  printf("%s\n",buffer);
  }
  Close(fd2, cl);
  Delete("File1",cl);
  List(cl);
}
