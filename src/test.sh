#/usr/bin/ bash 
# clean the directory of objects and executables, then recompile
. compile.sh

# start the server and place in the background
./server &

# start the client and connect to localhost
./client localhost


