all: client server
client : client.o sum_clnt.o  sum_xdr.o  
	gcc -o client client.o sum_clnt.o sum_xdr.o
server: sum_xdr.o  sum_svc.o server.o
	gcc -o server server.o sum_svc.o sum_xdr.o 
client.o: sum.h client.c 
	gcc -c client.c
server.o: sum.h server.c
	gcc -c server.c
sum_svc.o: sum.h sum_svc.c 
	gcc -c sum_svc.c
sum_clnt.o: sum_clnt.c sum.h
	gcc -c sum_clnt.c 
sum_xdr.o: sum_xdr.c sum.h
	gcc -c sum_xdr.c

clean :
	rm  *.o *~

 
