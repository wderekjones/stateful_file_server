/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "ssnfs.h"

bool_t
xdr_open_input(xdrs, objp)
	XDR *xdrs;
	open_input *objp;
{

	if (!xdr_vector(xdrs, (char *)objp->user_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	if (!xdr_vector(xdrs, (char *)objp->file_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_open_output(xdrs, objp)
	XDR *xdrs;
	open_output *objp;
{

	if (!xdr_int(xdrs, &objp->fd))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_read_input(xdrs, objp)
	XDR *xdrs;
	read_input *objp;
{

	if (!xdr_vector(xdrs, (char *)objp->user_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->fd))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->numbytes))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_read_output(xdrs, objp)
	XDR *xdrs;
	read_output *objp;
{

	if (!xdr_array(xdrs, (char **)&objp->out_msg.out_msg_val, (u_int *)&objp->out_msg.out_msg_len, ~0, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_write_input(xdrs, objp)
	XDR *xdrs;
	write_input *objp;
{

	if (!xdr_vector(xdrs, (char *)objp->user_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->fd))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->numbytes))
		return (FALSE);
	if (!xdr_array(xdrs, (char **)&objp->buffer.buffer_val, (u_int *)&objp->buffer.buffer_len, ~0, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_write_output(xdrs, objp)
	XDR *xdrs;
	write_output *objp;
{

	if (!xdr_array(xdrs, (char **)&objp->out_msg.out_msg_val, (u_int *)&objp->out_msg.out_msg_len, ~0, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_list_input(xdrs, objp)
	XDR *xdrs;
	list_input *objp;
{

	if (!xdr_vector(xdrs, (char *)objp->user_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_list_output(xdrs, objp)
	XDR *xdrs;
	list_output *objp;
{

	if (!xdr_array(xdrs, (char **)&objp->out_msg.out_msg_val, (u_int *)&objp->out_msg.out_msg_len, ~0, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_delete_input(xdrs, objp)
	XDR *xdrs;
	delete_input *objp;
{

	if (!xdr_vector(xdrs, (char *)objp->user_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	if (!xdr_vector(xdrs, (char *)objp->file_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_delete_output(xdrs, objp)
	XDR *xdrs;
	delete_output *objp;
{

	if (!xdr_array(xdrs, (char **)&objp->out_msg.out_msg_val, (u_int *)&objp->out_msg.out_msg_len, ~0, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_close_input(xdrs, objp)
	XDR *xdrs;
	close_input *objp;
{

	if (!xdr_vector(xdrs, (char *)objp->user_name, 10, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->fd))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_close_output(xdrs, objp)
	XDR *xdrs;
	close_output *objp;
{

	if (!xdr_array(xdrs, (char **)&objp->out_msg.out_msg_val, (u_int *)&objp->out_msg.out_msg_len, ~0, sizeof(char), (xdrproc_t)xdr_char))
		return (FALSE);
	return (TRUE);
}