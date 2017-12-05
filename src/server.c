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

struct server_state { // contains a list of the open files, position i corresponds to file descriptor i
  struct {
		int fd_state_val[100];
	}fd_state;
  struct {
    char name_table_val[100][10]; // create an array of 100 elements to 10 element long strings
  }name_table;
  int head_pos[100]; // keep track of the current head positions for each of the files
};
typedef struct server_state server_state;

server_state master_state; // may need to create a pointer and malloc some memory....

void update_state(file_info *fi, int is_open)
{
  int key = fi->fd;
  master_state.fd_state.fd_state_val[key] = is_open;
  strcpy(master_state.name_table.name_table_val[key], fi->filename);
  master_state.head_pos[key] = 0;
}


char * get_file_name(int key)
{
  return master_state.name_table.name_table_val[key];
}

int get_file_status(int key)
{
  return master_state.fd_state.fd_state_val[key];
}

int get_file_offset(int key)
{
  return master_state.head_pos[key];
}

void set_file_offset(int key, int offset_val)
{
  master_state.head_pos[key] = offset_val;
}

void close_file_state(int key)
{
   master_state.fd_state.fd_state_val[key] = 0; // update the file descriptor to denote closed
   master_state.head_pos[key] = 0; // reset the position of read/write head
}

int file_exists_in_state(int key)
{
  if(master_state.name_table.name_table_val[key] == NULL)
  {
      return 0;
  }
  else{
    return 1;
  }
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
    exists = 0;
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


void get_file_info(char *username, char *filename, file_info *fi) {
    int fd, exists;
    fd = open("files.dat", O_RDONLY);
    for (exists = 0; read(fd, fi, sizeof(file_info)) > 0;) {
        if ((strcmp(username, fi->username) == 0) && (strcmp(filename, fi->filename) == 0)) {
            exists = 1;
            break;
        }
    }
    close(fd);
}


// this function searches for a closed file and assigns a new file descriptor so that it may be read/written to
void open_closed_file(char* closed_username, char* closed_filename, int reopen_fd)
{
  file_info fi;
  strcpy(fi.username, closed_username);
  strcpy(fi.filename, closed_filename);
  get_file_info(closed_username, closed_filename, &fi);
  for (int i = 0; i < 100; i++)
  {
    if( (strcmp(master_state.name_table.name_table_val[i],closed_filename) == 0) && (master_state.fd_state.fd_state_val[i] == 0) )
    {
      master_state.fd_state.fd_state_val[i] = 1;
    }
  }
  fi.fd = reopen_fd;
  change_file_info(&fi);
}

int get_open_status_from_name(char* closed_filename)
{
  int status = 0;

  for (int i = 0; i < 100; i++)
  {
    if(strcmp(master_state.name_table.name_table_val[i], closed_filename) ==0)
    {
      return master_state.fd_state.fd_state_val[i];
    }
  }
  return status;
}

// RPC call "open".
open_output * open_file_1_svc(open_input *inp, struct svc_req *rqstp)
{
    int page;
    file_info fi;
    char message[512];
    static int fd = 1;
    printf("open_file_1_svc: (user_name = '%s', file_name = '%s')\n", inp->user_name, inp->file_name);
    init_disk();

    printf("file exists %d\n", file_exists(inp->user_name, inp->file_name));
    printf("get_open_status_from_name: %d\n", get_open_status_from_name(inp->file_name));
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
    else if (file_exists(inp->user_name,inp->file_name) && !(get_open_status_from_name(inp->file_name)))
    {
      strcpy(message,"Error: File has been closed.");
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
    change_file_info(&fi);
    update_state(&fi,1);
    return &out;
}


// RPC call "read".
read_output * read_file_1_svc(read_input *inp, struct svc_req *rqstp)
{
    char *message, *buffer;
    int offset, numbytes, at, page_index, page_num, len, fd, buffer_size, message_size, read_fail;
    file_info fi;
    init_disk();
    printf("read_file_1_svc: (user_name = %s, fd = %i, numbytes = %i)\n",
           inp->user_name, inp->fd, inp->numbytes);
    static read_output out;

    // update the members of the file_info struct
    char *file_name = get_file_name(inp->fd);
    strcpy(fi.filename,file_name);
    strcpy(fi.username, inp->user_name);
    fi.fd = inp->fd;
    offset = get_file_offset(fi.fd); //set the file offset to the current value for the given file descriptor
    if (file_exists(inp->user_name,file_name) && !(get_open_status_from_name(file_name)))
    {
      strcpy(message,"Error: File has been closed.");
    }
    else if (file_exists_in_state(inp->fd) && (get_file_status(inp->fd) > 0)) {
        numbytes = inp->numbytes;
        buffer_size = numbytes + 1;
        buffer = malloc(buffer_size);
        char *reply = "Content read: ";
        message_size = strlen(reply) + buffer_size;
        get_file_info(inp->user_name, file_name, &fi);
        at = 0;
        while (at < numbytes) {
          printf("at = %d\n", at);
            page_index = offset / PAGE_SIZE;
            if (page_index >= fi.used)
            {
              read_fail = 1;
              break;
            }
            page_num = fi.pages[page_index];
            if ((numbytes - at) < (PAGE_SIZE - (offset % PAGE_SIZE)))
            {
                len = numbytes - at;
            }
            else
            {
                    len = PAGE_SIZE - (offset % PAGE_SIZE);
            }
            fd = open("disk.dat", O_RDONLY);

            lseek(fd, (PAGE_SIZE * page_num) + (offset % PAGE_SIZE), SEEK_SET);
            printf("len = %d\n", len);
            read(fd, buffer+at, len); // this is given segfaults....
            close(fd);
            at += len;
            offset+=len;
            printf("%s\n", buffer);
            set_file_offset(fi.fd,offset); //update the offset value

          }
          buffer[numbytes] = '\x00';
          message = malloc(512);
          strcpy(message,buffer);
        }
    else
    {
        // file doesn't exist
        message = malloc(512);
        memset(message,0,message_size);
        snprintf(message, 512, "Error: file %s does not exist.", fi.filename);
    }
    out.out_msg.out_msg_len = strlen(message) + 1;
    out.out_msg.out_msg_val = strdup(message);
    return &out;
}


// RPC call "write".
write_output * write_file_1_svc(write_input *inp, struct svc_req *rqstp)
{
    char message[512];
    char *buffer;
    file_info fi;
    int fd, numbytes, offset, size, at, page_index, page_num, len;
    init_disk();
    printf("write_file_1_svc: (user_name = '%s', file_descriptor = '%i' numbytes = %d)\n",
           inp->user_name, inp->fd, inp->numbytes);
    printf("write buffer: %s\n", inp->buffer.buffer_val);
    static write_output out;
    char *file_name = get_file_name(inp->fd);
    strcpy(fi.filename, file_name);
    offset = get_file_offset(fi.fd); //set the file offset to the current value for the given file descriptor
    if (file_exists(inp->user_name,file_name) && !(get_open_status_from_name(file_name)))
    {
      strcpy(message,"Error: File has been closed.");
    }
    if (file_exists_in_state(inp->fd) && (get_file_status(inp->fd) > 0)) {
        numbytes = inp->numbytes;
        buffer = inp->buffer.buffer_val;
        get_file_info(inp->user_name,file_name, &fi);
        if (offset > (fi.used * PAGE_SIZE - 1)) {
            printf("used = %d\n", fi.used);
            snprintf(message, 512, "Error: writing past end of file.");
        }
        else if ((offset + numbytes) > (PAGE_SIZE * MAX_PAGES))
        {
            snprintf(message, 512, "Error: write is too large.");
        }
        else
        {
          at = 0;
          while (at < numbytes) {
              page_index = offset / PAGE_SIZE;
              if (page_index == fi.used)
              {
                page_num = get_free_page();
                fi.pages[fi.used] = page_num;
                fi.used++;
                change_file_info(&fi);
              }
              else{
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
            printf("lseek to %d\n", (PAGE_SIZE * page_num) + (offset % PAGE_SIZE));
            printf("%d %d %d\n", PAGE_SIZE, page_num, offset%PAGE_SIZE);
            lseek(fd, (PAGE_SIZE * page_num), SEEK_SET);
            // lseek(fd, (PAGE_SIZE * page_num) + (offset % PAGE_SIZE), SEEK_SET);
            write(fd, buffer+at, len);
            close(fd);
            at += len;
            offset += len;
            set_file_offset(fi.fd,offset); //update the offset value
          }
            snprintf(message, 512, "%d characters written to file %s", numbytes, fi.filename);
        }
    }
    else
    {
        // file doesn't exist
        snprintf(message, 512, "Error: file %s does not exist.", fi.filename);
    }
    out.out_msg.out_msg_len = strlen(message) + 1;
    out.out_msg.out_msg_val = strdup(message);
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
  file_info fi;
  strcpy(fi.username,inp->user_name);
  filename = get_file_name(inp->fd);
  strcpy(fi.filename,filename);
  get_file_info(fi.username, filename, &fi);
  // make call to function that searches the file table and changes the file descriptor of the input file to denote that it is closed
  file_close(inp->user_name, inp->fd);
  close_file_state(inp->fd);
  snprintf(message,512, "user '%s' closed: '%i'", inp->user_name, inp->fd);
  printf("%s\n", message);
  static close_output out;
  out.out_msg.out_msg_len = strlen(message) + 1;
  out.out_msg.out_msg_val = strdup(message);
  return &out;
}
