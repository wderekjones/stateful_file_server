/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "ssnfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
ssnfsprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		open_input open_file_1_arg;
		read_input read_file_1_arg;
		write_input write_file_1_arg;
		list_input list_files_1_arg;
		delete_input delete_file_1_arg;
		close_input close_file_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case open_file:
		_xdr_argument = (xdrproc_t) xdr_open_input;
		_xdr_result = (xdrproc_t) xdr_open_output;
		local = (char *(*)(char *, struct svc_req *)) open_file_1_svc;
		break;

	case read_file:
		_xdr_argument = (xdrproc_t) xdr_read_input;
		_xdr_result = (xdrproc_t) xdr_read_output;
		local = (char *(*)(char *, struct svc_req *)) read_file_1_svc;
		break;

	case write_file:
		_xdr_argument = (xdrproc_t) xdr_write_input;
		_xdr_result = (xdrproc_t) xdr_write_output;
		local = (char *(*)(char *, struct svc_req *)) write_file_1_svc;
		break;

	case list_files:
		_xdr_argument = (xdrproc_t) xdr_list_input;
		_xdr_result = (xdrproc_t) xdr_list_output;
		local = (char *(*)(char *, struct svc_req *)) list_files_1_svc;
		break;

	case delete_file:
		_xdr_argument = (xdrproc_t) xdr_delete_input;
		_xdr_result = (xdrproc_t) xdr_delete_output;
		local = (char *(*)(char *, struct svc_req *)) delete_file_1_svc;
		break;

	case close_file:
		_xdr_argument = (xdrproc_t) xdr_close_input;
		_xdr_result = (xdrproc_t) xdr_close_output;
		local = (char *(*)(char *, struct svc_req *)) close_file_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (SSNFSPROG, SSNFSVER);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, SSNFSPROG, SSNFSVER, ssnfsprog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (SSNFSPROG, SSNFSVER, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, SSNFSPROG, SSNFSVER, ssnfsprog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (SSNFSPROG, SSNFSVER, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
