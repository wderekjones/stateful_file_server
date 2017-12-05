#include <stdio.h> // For printf, etc.
#include <stdlib.h> // For atoi, etc.
#include <string.h> // Needed for various string manipulations.
#include <rpc/rpc.h> // Header for RPC calls.
#include "ssnfs.h" // Header for the RPC service created by rpcgen.
#include <readline/readline.h> // Readline is required for the main interactive loop.
#include <readline/history.h> // Needed for prompt history
#include <pwd.h> // Need to get username.


CLIENT *cl; // Pointer to the client handle.


// int Open(char *filename, CLIENT *cl)
int Open(char *filename)
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
  else if((outp->fd) > 0){
    printf("%d opened successfully.\n", outp->fd);
  }
  else if((outp->fd) == -3){
    printf("file exists.\n");
  }
  else{
    printf("open unsuccessful.\n");
  }
  return outp->fd;
}


// void Read(int fd, char *buffer, int numbytes, CLIENT *cl)
void Read(int fd, char *buffer, int numbytes)
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

// void Write(int fd, char *buffer, int numbytes, CLIENT *cl)
void Write(int fd, char *buffer, int numbytes)
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

// void Close(int fd, CLIENT *cl)
void Close(int fd)
{
  char *username = getpwuid(getuid())->pw_name; // Get the user's username.
  close_input in;
  strcpy(in.user_name, username); // Copy user name.
  in.fd = fd; // Copy the file descriptor
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

// void Delete(char *filename, CLIENT *cl)
void Delete(char *filename)
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

// void List(CLIENT *cl)
void List()
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
  cl = clnt_create(argv[1], SSNFSPROG, SSNFSVER, "tcp"); // Create the RPC client.
  if (cl == NULL)
  { // If there was an error creating the client...
      fprintf(stderr, "getting client handle failed"); // Report an error message.
      exit(2); // And exit with an error code.
  }



  int i,j,k;
  int fd1,fd2,fd3;
  char buffer[100];
  fd1=Open("File1"); // opens the file
  for (i=0; i< 20;i++){
  Write(fd1, "funny contents in the file 1", 20);
  }
  Close(fd1);

  fd2=Open("File2");
  for (j=0; j< 20;j++){
  Read(fd2, buffer, 1);
  // printf("%s\n",buffer);
  }
  Close(fd2);
  Delete("File1");
  List();
}

