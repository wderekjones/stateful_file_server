#include <stdio.h>
#include <rpc/rpc.h>
#include "sum.h"


    
sum_out * sum_proc_1_svc(range_in *inp, struct svc_req *rqstp)
  {
   int i;
   static sum_out out; //important - return variable should be static
   out.result=0;
   for (i=inp->min; i<=inp->max; i++)
     out.result+= i;
   return &out;
  }
