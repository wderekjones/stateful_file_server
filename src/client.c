#include <stdio.h> // For printf, etc.
#include <stdlib.h> // For atoi, etc.
#include <string.h> // Needed for various string manipulations.
#include <rpc/rpc.h> // Header for RPC calls.
#include "ssnfs.h" // Header for the RPC service created by rpcgen.
#include <readline/readline.h> // Readline is required for the main interactive loop.
#include <readline/history.h> // Needed for prompt history
#include <pwd.h> // Need to get username.

int main(int argc, char **argv)
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

    char * username = getpwuid(geteuid())->pw_name; // Get the user's username.

    // Main Interactive Loop.
    char *linebuffer, *command, *filename, *tofilename;  // Strings for the line buffer, command, filename...
    int valid_command; // Flag to determine if the user entered a valid command.
    // void *outp; // All the output structs have the same structure, just use a single pointer.
    write_output *outp;
    char *offset, *numbytes, *buffer; // Parameters to read and write.
    for (;;) { // Loop until user enters EOF.
        linebuffer = readline("> "); // Prompt the user for a command.
        if (!linebuffer) // If the user enters and EOF...
            break; // exit the interactive loop with no error.
        add_history(linebuffer); // Add input to history.
        command = strtok(linebuffer, " "); // Split off the first word, the command.
        if (command) { // The line isn't blank...
            if (strcmp(command, "open") == 0)
            { // If the user enters the "create" command...
                write_input in; // Input struct for the RPC call.
                filename = strtok(NULL, " "); // Split off the name of the file to be created.
                if (filename)
                { // If the user does enter a filename...
                    strcpy(in.user_name, username); // Copy user name.
                    outp = open_file_1(&in, cl); // And call the create file RPC.
                    valid_command = 1; // User entered a valid command.
                }
                else
                { // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
                }
            }
            else if (strcmp(command, "read") == 0)
            { // If the user enters the "read" command...
                read_input in; // Input struct for the RPC call.
                filename = strtok(NULL, " "); // Split off the name of the file to be read.
                if (filename)
                { // If the user does enter a filename...
                    offset = strtok(NULL, " "); // Split off the offset.
                    if (offset)
                    { // If the user does enter a filename...
                        numbytes = strtok(NULL, " "); // Split off the offset.
                        if (numbytes)
                        { // If the user does enter a filename...
                            strcpy(in.user_name, username); // Copy user name.
                            outp = read_file_1(&in, cl); // And call the read file RPC.
                            valid_command = 1; // User entered a valid command.
                        }
                        else
                        {
                            printf("Number of bytes required.\n"); // Report an error message.
                        }
                    }
                    else
                    { // If the user doesn't enter a filename...
                        printf("Offset required.\n"); // Report an error message.
                    }
                }
                else
                { // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
                }
            }
            else if (strcmp(command, "write") == 0)
            { // If the user enters the "write" command...
                write_input in; // Input struct for the RPC call.
                strcpy(in.user_name, username); // Copy user name.
                filename = strtok(NULL, " "); // Split off the name of the file to which we will write.
                if (filename)
                { // If the user does enter a filename...
                    offset = strtok(NULL, " "); // Split off offset of writing.
                    if (offset)
                    { // If the user does enter a filename...
                        numbytes = strtok(NULL, " "); // Split off the length of the write.
                        if (numbytes)
                        { // If the user does enter an offset...
                            buffer = strtok(NULL, " "); // Split off the length of the write.
                            if (buffer)
                            {
                                strcpy(in.user_name, username); // Copy user name.
                                in.numbytes = atoi(numbytes); // The number of bytes to write.
                                in.buffer.buffer_val = strdup(buffer);
                                in.buffer.buffer_len = strlen(buffer);
                                outp = write_file_1(&in, cl); // And call the create file RPC.
                                valid_command = 1; // User entered a valid command.
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
                        printf("Offset required.\n"); // Report an error message.
                    }
                }
                else
                { // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
                }
            }
            else if (strcmp(command, "list") == 0)
            { // If the user enters the "list" command...
                list_input in; // Input struct for the RPC call.
                strcpy(in.user_name, username); // Copy user name.
                outp = list_files_1(&in, cl); // And call the list file RPC.
                valid_command = 1; // User entered a valid command.
            }
            else if (strcmp(command, "delete") == 0)
            { // If the user enters the "delete" command...
                delete_input in; // Input struct for the RPC call.
                filename = strtok(NULL, " "); // Split off the name of the file to be deleted.
                if (filename)
                { // If the user does enter a filename...
                    strcpy(in.user_name, username); // Copy user name.
                    strcpy(in.file_name, filename); // Copy file name.
                    outp = delete_file_1(&in, cl); // And call the create file RPC.
                    valid_command = 1; // User entered a valid command.
                }
                else
                { // If the user doesn't enter a filename...
                    printf("Filename required.\n"); // Report an error message.
                }
            }
            else if (strcmp(command, "close") == 0)
            {
                close_input in; // Input struct for the RPC call.
                filename = strtok(NULL, " "); // Split off the name of the file to be deleted.
                if (filename)
                { // If the user does enter a filename...
                    strcpy(in.user_name, username); // Copy user name.
                    outp = close_file_1(&in, cl); // And call the close file RPC.
                    valid_command = 1; // User entered a valid command.
                }
            }
            else
            { // User did not enter a recognized command.
                printf("Unrecognized command.\n");
            } // End of command identification branch.
        if (valid_command)
        {
            if (outp == NULL)
            { // If the RPC call fails...
                fprintf(stderr, "remote procedure call failed"); // Report an error message.
                exit(3); // And exit with an error code.
            }
            else
            {
              char * output = malloc((outp->out_msg.out_msg_len + 1));
              memset(output, 0, (outp)->out_msg.out_msg_len + 1);
              strncpy(output, (outp)->out_msg.out_msg_val, (outp)->out_msg.out_msg_len + 1);
              printf("%s\n", (outp)->out_msg.out_msg_val); // Print the result of the RPC call.
              printf("%s\n", output); // Print the result of the RPC call.
            }
        } // End of command processing branch.
      }
      valid_command = 0; // Reset flag.
      free(linebuffer); // Free the buffer allocated by readline.
    }
    printf("\n"); // Make terminal look nice.
    exit(0); // End the program with no error.
}
