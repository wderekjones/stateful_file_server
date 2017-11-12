

#include <rpc/rpc.h>
#include "sum.h"
int
main(int argc, char **argv)
{
  CLIENT *cl;
  range_in in;
  sum_out *outp;
  if (argc!=4){
    fprintf(stderr, "usage: %s <hostname> <first integer> <last integer>\n",argv[0]);
    exit(1);
  }
  cl= clnt_create(argv[1], SUM_PROG,SUM_VERS,"tcp");
  if (cl==NULL){
    fprintf(stderr, "getting client handle failed");
    exit(2);
  }
  in.min=atol(argv[2]);
  in.max=atol(argv[3]);
  outp = sum_proc_1(&in,cl); 
  if (outp==NULL){
    fprintf(stderr, "remote procedure call failed");
    exit(3);
  }
  printf("The sum of all numbers from %ld to %ld is: %ld\n", in.min, in.max,outp->result);
  exit(0);
}
