
struct range_in{
  long min;
  long max;
};

struct sum_out{
  long result;
};

program SUM_PROG{
   version SUM_VERS{
          sum_out SUM_PROC(range_in) = 1; /*Procedure number =1.*/
     }= 1;    /* version number */
}= 0x31110000; /* program number */





