#include <stdio.h> // For printf, etc.
#include <rpc/rpc.h> // For RPC facilities.
#include <string.h> // For strcpy, strcmp, strdup, strlen, etc.
#include "ssnfs.h" // Automatically generated rpc service header.
#include <unistd.h> // Needed for access, etc.
#include <fcntl.h> // Needed for ftruncate, etc.
#include <errno.h> // Need for errno, strerror, etc.
#include <stdlib.h>

#define MAX_FILES 100 // The maximum number of files in the filesystem.
#define MAX_PAGES 8 // The maximum number of pages per file.
#define BLOCK_SIZE 512 // Blocks are 512 bytes.
#define PAGE_SIZE (BLOCK_SIZE*8) // Pages are 8 blocks.

struct file_info { // Holds the file information.
    char username[10]; // Username of the file's owner.
    char filename[10]; // Name of the file.
    int fd;
    int used; // Number of pages used.
    int pages[MAX_PAGES]; // A list of the page numbers used by the file.
}; // End of struct file_info.
typedef struct file_info file_info; // Define as a type.

struct open_file_list { // contains a list of the open files
  struct {
		int fd_table_val[1000];
	}fd_table;
};
typedef struct open_file_list open_file_list;


void init_open_file_list(open_file_list *ofl)
{
  for (int i =0; i < sizeof(ofl->fd_table.fd_table_val); i++)
  {
    ofl->fd_table.fd_table_val[i] = -1; //default value to denote empty space
  }
}

void add_to_file_list(int fd, open_file_list *ofl)
{
  for (int i =0; i < sizeof(ofl->fd_table.fd_table_val); i++)
  {
    if(ofl->fd_table.fd_table_val[i] < 0)
    {
      ofl->fd_table.fd_table_val[i] = fd;
    }
  }
}


int check_file_list(int fd, open_file_list *ofl)
{
  for (int i=0; i < sizeof(ofl->fd_table.fd_table_val); i++)
  {
    if(ofl->fd_table.fd_table_val[i] == fd)
    {
      return 1; // return that the file was successfully found in the table
    }
  }
  return 0; // all other cases did not find the file
}

// Initializes the virtual disk files if they do not all exist already.
int init_disk() {
    if ((access("files.dat", F_OK) == 0) && (access("pages.dat", F_OK) == 0) && (access("disk.dat", F_OK) == 0)) {
    } else {
        ftruncate(creat("disk.dat", 0666), 1024*1024*10);
        ftruncate(creat("pages.dat", 0666), 2560);
        ftruncate(creat("files.dat", 0666), sizeof(file_info) * MAX_FILES);
    }
}

// Obtain a free page for a file to use.  Returns page number.
int get_free_page() {
    int fd;
    int i;
    char flag;
    fd = open("pages.dat", O_RDWR);
    if (fd == -1) // if fd( < 0)
        printf("%s\n", strerror(errno));
    read(fd, &flag, 1); // what is flag?
    for (i = 0; flag; i++) {
        if (read(fd, &flag, 1) == -1) { //if unable to read
            printf("%s\n", strerror(errno));
            break;
        }
    }
    lseek(fd, -1, SEEK_CUR);
    write(fd, "\xff", 1);
    close(fd);
    return i;
}

// Checks if a file exists.  Return 0 on false, 1 on true.
int file_exists(char *username, char *filename) {
    int fd, exists;
    file_info fi;
    fd = open("files.dat", O_RDONLY);
    for (exists = 0; read(fd, &fi, sizeof(fi)) > 0;) {
        if ((strcmp(username, fi.username) == 0) && (strcmp(filename, fi.filename) == 0)) {
            exists = 1;
            break;
        }
    }
    close(fd);
    return exists;
}


// Get the file name from file descriptor.  Takes the username, file descriptor.
char * get_file_name(char *username, int target_fd) {
    int fd;
    int exists = 0;
    char filebuf[10];
    char *fp = filebuf;
    file_info fi;
    fd = open("files.dat", O_RDONLY);
    for (exists = 0; read(fd, &fi, sizeof(fi)) > 0;) {
        if ((strcmp(username, fi.username) == 0) && ((target_fd == fi.fd) == 1)) {
            // strcpy(filebuf,fi.filename);
            strcpy(fp,fi.filename);
            // printf("fi.filename: %s \t fp: %s\n", fi.filename, fp);
        }
    }
    close(fd);
    return fp;
}

// get file descriptor from file name
int get_file_desc(char *username, char *filename) {
    int fd;
    int out_fd = -1;
    int exists = 0;
    char filebuf[10];
    char *fp = malloc(512); // probably don't need this much space
    memset(fp,0,512);
    file_info fi;
    fd = open("files.dat", O_RDONLY);
    for (exists = 0; read(fd, &fi, sizeof(file_info)) > 0;) {
        if ((strcmp(username, fi.username) == 0) && ((filename == fi.filename) == 1)) {
          out_fd = fi.fd;
        }
    }
    close(fd);
    return out_fd;
}



// Commit changes to a file_info struct.  Takes only a file_info struct pointer as input.
void change_file_info(file_info *fi) {
    int fd, exists;
    file_info block;
    fd = open("files.dat", O_RDWR);
    for (exists = 0; read(fd, &block, sizeof(file_info)) > 0;) {
        if ((strcmp(fi->username, block.username) == 0) && (strcmp(fi->filename, block.filename) == 0)) {
            exists = 1;
            break;
        }
    }
    if (exists) {
        lseek(fd, -sizeof(file_info), SEEK_CUR);
        printf("used %d\n", fi->used);
        printf("write %zd\n", write(fd, fi, sizeof(file_info)));
    }
    close(fd);
}

// Inserts a new file_info struct into the file table.
int add_file(file_info fi) {
    int fd;
    int found;
    file_info block;
    // search for an empty block
    fd = open("files.dat", O_RDWR);
    for (found = 0; read(fd, &block, sizeof(block)) > 0;) {
        if (block.username[0] == 0) {
            found = 1;
            break;
        }
    }
    // insert file
    if (found) {
        // why is the buffer allowed to be negative?
        lseek(fd, -sizeof(fi), SEEK_CUR);
        printf("write %zd\n", write(fd, &fi, sizeof(fi)));
    }
    close(fd);
    // return success
    return found;
}

// Write a string with a list of files owned by a user to the buffer.
// Buffer must be at least 512 bytes!
void file_list(char *username, char *buffer) {
    memset(buffer, 0 , 512);
    int fd;
    file_info fi;
    fd = open("files.dat", O_RDONLY);
    while (read(fd, &fi, sizeof(fi)) > 0) {
        if (strcmp(username, fi.username) == 0) {
            strcat(buffer, " ");
            strcat(buffer, fi.filename);
              // add things here to change the output of the file list
        }
    }
    close(fd);
}

// Free a page with given page number by setting its entry in the page table to 0.
void free_page(int page_index) {
    int fd;
    fd = open("pages.dat", O_RDWR);
    lseek(fd, page_index, SEEK_SET);
    write(fd, "\x00", 1);
    close(fd);
}

// Delete a file owned by a user with filename as its name.
int file_delete(char *username, char *filename) {
    int fd;
    int found, i;
    file_info fi;
    // search for file's file info block
    fd = open("files.dat", O_RDWR);
    for (found = 0; read(fd, &fi, sizeof(fi)) > 0;) {
        if ((strcmp(username, fi.username) == 0) && (strcmp(filename, fi.filename) == 0)) {
            found = 1;
            break;
        }
    }
    // delete file
    if (found) {
        // free disk pages
        for (i = 0; i < fi.used; i++) {
            free_page(fi.pages[i]);
        }
        lseek(fd, -sizeof(fi), SEEK_CUR);
        memset(&fi, 0, sizeof(fi));
        printf("write %zd\n", write(fd, &fi, sizeof(fi)));
    }
    close(fd);
    // return success
    return found;
}


// closes a file owned by a user given the file descriptor value of the target file
void file_close(char *username, int target_fd) {
    int fd;
    int found, i;
    file_info fi;
    // search for file's file info block
    fd = open("files.dat", O_RDWR);
    for (found = 0; read(fd, &fi, sizeof(fi)) > 0;) {
        if ((strcmp(username, fi.username) == 0) && (target_fd == fi.fd)) {
            found = 1;
            fi.fd = -2; // this denotes that the file has been closed
            change_file_info(&fi); // commit the changes
            break;
        }
    }
    close(fd);
    // return success
}


int get_file_status(char *username, char* filename)
{
   int fd;
   int found, i;
   file_info fi;
   // search for file's file info block
   fd = open("files.dat", O_RDWR);
   for (found = 0; read(fd, &fi, sizeof(fi)) > 0;) {
     printf("%i\n", i);
       if ((strcmp(username, fi.username) == 0) && (strcmp(filename, fi.filename) == 0))
       {
           found = 1;
           return fi.fd;
          // break;
       }
       i++;
   }
   close(fd);
   // return success
   return fi.fd;
}


// RPC call "open".
open_output * open_file_1_svc(open_input *inp, struct svc_req *rqstp)
{

    // TODO: check to see if file exists, if it does than get its file descriptor and reuse it
    // TODO: once file has been opened, check to see if its descriptor is contained in the file table else add it
    int page;
    file_info fi;
    char message[512];
    static int fd = 1;
    printf("open_file_1_svc: (user_name = '%s', file_name = '%s')\n", inp->user_name, inp->file_name);
    init_disk();

    printf("file exists %d\n", file_exists(inp->user_name, inp->file_name));
    static open_output out;
    out.fd = -1; // assume that the open call is unsuccessful in order to simplify

    if (!file_exists(inp->user_name, inp->file_name))
    {
        strcpy(fi.username, inp->user_name);
        strcpy(fi.filename, inp->file_name);
        fi.used = 1;
        fi.fd = fd;
        page = get_free_page();

        if (page != -1)
        {
            fi.pages[0] = page;
        }
        else
        {
            printf("no pages left");
        }

        if (add_file(fi))
        {
            printf("%s created for user %s\n", inp->file_name, inp->user_name);
            out.fd = fd;
        }
        else
        {
            strcpy(message, "Error: Max number of files reached.");
        }

    }
    else
    {
        strcpy(message, "Error: file already exists");
    }

    // increment the file descriptor for the next call and return the output file descriptor
    if (out.fd != -1)
    {
      fd +=1;
    }
    return &out;
}


// RPC call "read".
read_output * read_file_1_svc(read_input *inp, struct svc_req *rqstp)
// read_output * read_file_1_svc(read_input *inp)
{
    // TODO: check open file table to s
    // TODO: when performing the read, need to check current position (read x bytes from [cur_pos - x] to [cur_pos])

    char message[512];
    char *buffer; // using a fixed buffer to make things easy, adjust this to allow one to write the entire file at once

    // offset is where to begin the read, at is the current position of the client from previous reads
    int offset, numbytes, at, page_index, page_num, len, fd, buffer_size, message_size, read_fail;
    file_info fi;
    static int cur_pos = 0;

    // get_file_info(inp->user_name,inp->fd,fi);

    printf("read_file_1_svc: (user_name = %s, fd = %i, numbytes = %i)\n",
           inp->user_name, inp->fd, inp->numbytes);
    static read_output out;
    char *file_name = get_file_name(inp->user_name,inp->fd);
    strcpy(fi.filename,file_name);
    if (file_exists(inp->user_name, fi.filename) && (get_file_status(inp->user_name, file_name) > 0))
    {
        // offset = inp->offset;
        offset = 0;
        numbytes = inp->numbytes;
        printf("number of bytes to read: %i\n", numbytes);
        at = 0;
        buffer_size = numbytes + 1;
        buffer = malloc(buffer_size);
        memset(buffer, 0, buffer_size);
        char *reply = "Content read: ";
        message_size = strlen(reply) + buffer_size;
        // get_file_info(inp->user_name, inp->fd, &fi);
        read_fail = 0;

        while (at < numbytes) {
            printf("at = %d\n", at);
            page_index = offset / PAGE_SIZE;
            if (page_index >= fi.used) {
                read_fail = 1;
                break;
            }

            page_num = fi.pages[page_index];
            if ((numbytes - at) < (PAGE_SIZE - (offset % PAGE_SIZE))) {
                len = numbytes - at;
            } else {
                len = PAGE_SIZE - (offset % PAGE_SIZE);
            }


            fd = open("disk.dat", O_RDONLY);
            // move to the current page that the file is being edited on
            // lseek(fd, (PAGE_SIZE * page_num), SEEK_SET);
            printf("PAGE_SIZE = %d \t page_num = %d\n",PAGE_SIZE, page_num );
            lseek(fd,(PAGE_SIZE * page_num), SEEK_CUR);

            printf("len = %d\n", len);
            if(
            read(fd, buffer, numbytes) < 0)
            {
              printf("an error occured during the read\n");
              exit(-1);
            }
            close(fd);
            at += len;
            offset += len;
            strncpy(message,buffer,512);
            printf("%s\n", buffer);
        }
        buffer[numbytes] = '\x00';
        if (read_fail) {
            // message = malloc(512);
            snprintf(message, 512, "Error: writing past end of file.");
            printf("%s\n", message);
        }
        else {
            // message = malloc(message_size);
            memset(message, 0, message_size);
            snprintf(message, message_size, "Content read: %s", buffer);
            printf("%s\n", message);
        }
    }
    else {
        // message = malloc(512);
        snprintf(message, 512, "Error: file %s does not exist.", fi.filename);
    }
    // strncpy(message,fi.filename,512);
    // strncpy(message,buffer,512);
    out.out_msg.out_msg_len = strlen(message) + 1;
    out.out_msg.out_msg_val = strdup(message);
    // free(buffer);
    // free(message);
    // printf("%s (%d)\n", out.out_msg.out_msg_val, out.out_msg.out_msg_len);
    return &out;
}


// RPC call "write".
write_output * write_file_1_svc(write_input *inp, struct svc_req *rqstp)
{

    // check

    char message[512];
    char *buffer;
    file_info fi;
    int fd, numbytes, offset, size, at, page_index, page_num, len;

    printf("write_file_1_svc: (user_name = '%s', file_descriptor = '%i' numbytes = %d)\n",
           inp->user_name, inp->fd, inp->numbytes);
    printf("write buffer: %s\n", inp->buffer.buffer_val);
    static write_output out;
    char *file_name = get_file_name(inp->user_name,inp->fd);
    printf("file_name: %s\n", file_name);
    printf("user_name: %s\n", inp->user_name);
    printf("file_status: %d\n", get_file_status(inp->user_name, file_name));
    if (file_exists(inp->user_name, file_name) && (get_file_status(inp->user_name, file_name) > 0)) {
        numbytes = inp->numbytes;
        buffer = inp->buffer.buffer_val;
        printf("used = %d\n", fi.used);

        at = 0;
        // while (at < numbytes) {
            // page_index = offset / PAGE_SIZE;
            if (page_index == fi.used)
            {
              page_num = get_free_page();
              fi.pages[fi.used] = page_num;
              fi.used++;
              change_file_info(&fi);
              page_num = fi.pages[page_index];
            }

            if ((numbytes - at) < (PAGE_SIZE - (offset % PAGE_SIZE)))
            {
                len = numbytes - at;
            }
            else
            {
                    len = PAGE_SIZE - (offset % PAGE_SIZE);
            }
            fd = open("disk.dat", O_RDWR);
            lseek(fd, (PAGE_SIZE * page_num), SEEK_SET);

            printf("lseek to %d\n", (PAGE_SIZE * page_num) + (offset % PAGE_SIZE));
            printf("%d %d %d\n", PAGE_SIZE, page_num, offset%PAGE_SIZE);
            // lseek(fd, (PAGE_SIZE * page_num) + (offset % PAGE_SIZE), SEEK_SET);
            write(fd, buffer+at, len);
            close(fd);
            at += len;

            snprintf(message, 512, "%d characters written to file %s", numbytes, fi.filename);
        }
    // }
    else
    {
        // file doesn't exist
        snprintf(message, 512, "Error: file %s does not exist.", fi.filename);
    }
    // strcpy(message,"testing write functionality");
    out.out_msg.out_msg_len = strlen(message) + 1;
    out.out_msg.out_msg_val = strdup(message);
    // free(&message);
    return &out;

}


// RPC call "list".
list_output * list_files_1_svc(list_input *inp, struct svc_req *rqstp)
{
    char message[512];
    char buffer[512];
    printf("list_file_1_svc: (username = '%s')\n", inp->user_name);
    file_list(inp->user_name, buffer);
    printf("files: %s\n", buffer);
    static list_output out;
    snprintf(message, 512, "The files are:%s", buffer);
    out.out_msg.out_msg_len = strlen(message) + 1;
    out.out_msg.out_msg_val = strdup(message);
    return &out;
}

// RCP call "delete".
delete_output * delete_file_1_svc(delete_input *inp, struct svc_req *rqstp)
{
    char message[512];
    printf("delete_file_1_svc: (user_name = '%s', file_name = '%s')\n", inp->user_name, inp->file_name);
    static delete_output out;
    if (file_exists(inp->user_name, inp->file_name))
    {
        file_delete(inp->user_name, inp->file_name);
        snprintf(message, 512, "%s deleted", inp->file_name);
    }
    else
    {
        // file doesn't exist
        snprintf(message, 512, "Error: file %s does not exist.", inp->file_name);
    }
    out.out_msg.out_msg_len = strlen(message) + 1;
    out.out_msg.out_msg_val = strdup(message);
    return &out;
}

// RPC call "close"
close_output * close_file_1_svc(close_input *inp, struct svc_req *rqstp)
{
  char message[512];
  int status = -1;
  char* filename;
  // make call to function that searches the file table and changes the file descriptor of the input file to denote that it is closed
  file_close(inp->user_name, inp->fd);
  snprintf(message,512, "user '%s' closed: '%i'", inp->user_name, inp->fd);
  printf("%s\n", message);
  static close_output out;
  out.out_msg.out_msg_len = strlen(message) + 1;
  out.out_msg.out_msg_val = strdup(message);
  return &out;
}
