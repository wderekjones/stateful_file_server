struct create_input
{
  char user_name[10];
  char file_name[10];
};

struct create_output
{
  char out_msg<>;
};

struct list_input
{
  char usrname[10];
};

struct list_output
{
  char out_msg<>;
};

struct delete_input
{
  char user_name[10];
  char file_name[10];
};

struct delete_output
{
   char out_msg<>;
}; 

struct write_input
{
  char user_name[10];
  char file_name[10];
  int offset;
  int numbytes;
  char buffer<>;
};

struct write_output
{
  char out_msg<>;
};

struct read_input
{
  char user_name[10];
  char file_name[10];
  int offset;
  int numbytes;
};

struct read_output
{
  char out_msg<>;
};

struct copy_input
{
  char user_name[10];
  char from_filename[10];
  char to_filename[10];
};

struct copy_output
{
  char out_msg<>;
};



program SSNFSPROG{
  version SSNFSVER{
    create_output create_file(create_input) = 1;
    list_output list_files(list_input) = 2; 
    delete_output delete_file(delete_input) = 3;
    write_output write_file(write_input) = 4;
    read_output read_file(read_input) = 5;
    copy_output copy_file(copy_input) = 6;
  } = 1;
}=0x31110023;

