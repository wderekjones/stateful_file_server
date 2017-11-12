/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SSNFS_H_RPCGEN
#define _SSNFS_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>


struct create_input {
	char user_name[10];
	char file_name[10];
};
typedef struct create_input create_input;
#ifdef __cplusplus
extern "C" bool_t xdr_create_input(XDR *, create_input*);
#elif __STDC__
extern  bool_t xdr_create_input(XDR *, create_input*);
#else /* Old Style C */
bool_t xdr_create_input();
#endif /* Old Style C */


struct create_output {
	struct {
		u_int out_msg_len;
		char *out_msg_val;
	} out_msg;
};
typedef struct create_output create_output;
#ifdef __cplusplus
extern "C" bool_t xdr_create_output(XDR *, create_output*);
#elif __STDC__
extern  bool_t xdr_create_output(XDR *, create_output*);
#else /* Old Style C */
bool_t xdr_create_output();
#endif /* Old Style C */


struct list_input {
	char usrname[10];
};
typedef struct list_input list_input;
#ifdef __cplusplus
extern "C" bool_t xdr_list_input(XDR *, list_input*);
#elif __STDC__
extern  bool_t xdr_list_input(XDR *, list_input*);
#else /* Old Style C */
bool_t xdr_list_input();
#endif /* Old Style C */


struct list_output {
	struct {
		u_int out_msg_len;
		char *out_msg_val;
	} out_msg;
};
typedef struct list_output list_output;
#ifdef __cplusplus
extern "C" bool_t xdr_list_output(XDR *, list_output*);
#elif __STDC__
extern  bool_t xdr_list_output(XDR *, list_output*);
#else /* Old Style C */
bool_t xdr_list_output();
#endif /* Old Style C */


struct delete_input {
	char user_name[10];
	char file_name[10];
};
typedef struct delete_input delete_input;
#ifdef __cplusplus
extern "C" bool_t xdr_delete_input(XDR *, delete_input*);
#elif __STDC__
extern  bool_t xdr_delete_input(XDR *, delete_input*);
#else /* Old Style C */
bool_t xdr_delete_input();
#endif /* Old Style C */


struct delete_output {
	struct {
		u_int out_msg_len;
		char *out_msg_val;
	} out_msg;
};
typedef struct delete_output delete_output;
#ifdef __cplusplus
extern "C" bool_t xdr_delete_output(XDR *, delete_output*);
#elif __STDC__
extern  bool_t xdr_delete_output(XDR *, delete_output*);
#else /* Old Style C */
bool_t xdr_delete_output();
#endif /* Old Style C */


struct write_input {
	char user_name[10];
	char file_name[10];
	int offset;
	int numbytes;
	struct {
		u_int buffer_len;
		char *buffer_val;
	} buffer;
};
typedef struct write_input write_input;
#ifdef __cplusplus
extern "C" bool_t xdr_write_input(XDR *, write_input*);
#elif __STDC__
extern  bool_t xdr_write_input(XDR *, write_input*);
#else /* Old Style C */
bool_t xdr_write_input();
#endif /* Old Style C */


struct write_output {
	struct {
		u_int out_msg_len;
		char *out_msg_val;
	} out_msg;
};
typedef struct write_output write_output;
#ifdef __cplusplus
extern "C" bool_t xdr_write_output(XDR *, write_output*);
#elif __STDC__
extern  bool_t xdr_write_output(XDR *, write_output*);
#else /* Old Style C */
bool_t xdr_write_output();
#endif /* Old Style C */


struct read_input {
	char user_name[10];
	char file_name[10];
	int offset;
	int numbytes;
};
typedef struct read_input read_input;
#ifdef __cplusplus
extern "C" bool_t xdr_read_input(XDR *, read_input*);
#elif __STDC__
extern  bool_t xdr_read_input(XDR *, read_input*);
#else /* Old Style C */
bool_t xdr_read_input();
#endif /* Old Style C */


struct read_output {
	struct {
		u_int out_msg_len;
		char *out_msg_val;
	} out_msg;
};
typedef struct read_output read_output;
#ifdef __cplusplus
extern "C" bool_t xdr_read_output(XDR *, read_output*);
#elif __STDC__
extern  bool_t xdr_read_output(XDR *, read_output*);
#else /* Old Style C */
bool_t xdr_read_output();
#endif /* Old Style C */


struct copy_input {
	char user_name[10];
	char from_filename[10];
	char to_filename[10];
};
typedef struct copy_input copy_input;
#ifdef __cplusplus
extern "C" bool_t xdr_copy_input(XDR *, copy_input*);
#elif __STDC__
extern  bool_t xdr_copy_input(XDR *, copy_input*);
#else /* Old Style C */
bool_t xdr_copy_input();
#endif /* Old Style C */


struct copy_output {
	struct {
		u_int out_msg_len;
		char *out_msg_val;
	} out_msg;
};
typedef struct copy_output copy_output;
#ifdef __cplusplus
extern "C" bool_t xdr_copy_output(XDR *, copy_output*);
#elif __STDC__
extern  bool_t xdr_copy_output(XDR *, copy_output*);
#else /* Old Style C */
bool_t xdr_copy_output();
#endif /* Old Style C */


#define SSNFSPROG ((rpc_uint)0x31110023)
#define SSNFSVER ((rpc_uint)1)

#ifdef __cplusplus
#define create_file ((rpc_uint)1)
extern "C" create_output * create_file_1(create_input *, CLIENT *);
extern "C" create_output * create_file_1_svc(create_input *, struct svc_req *);
#define list_files ((rpc_uint)2)
extern "C" list_output * list_files_1(list_input *, CLIENT *);
extern "C" list_output * list_files_1_svc(list_input *, struct svc_req *);
#define delete_file ((rpc_uint)3)
extern "C" delete_output * delete_file_1(delete_input *, CLIENT *);
extern "C" delete_output * delete_file_1_svc(delete_input *, struct svc_req *);
#define write_file ((rpc_uint)4)
extern "C" write_output * write_file_1(write_input *, CLIENT *);
extern "C" write_output * write_file_1_svc(write_input *, struct svc_req *);
#define read_file ((rpc_uint)5)
extern "C" read_output * read_file_1(read_input *, CLIENT *);
extern "C" read_output * read_file_1_svc(read_input *, struct svc_req *);
#define copy_file ((rpc_uint)6)
extern "C" copy_output * copy_file_1(copy_input *, CLIENT *);
extern "C" copy_output * copy_file_1_svc(copy_input *, struct svc_req *);

#elif __STDC__
#define create_file ((rpc_uint)1)
extern  create_output * create_file_1(create_input *, CLIENT *);
extern  create_output * create_file_1_svc(create_input *, struct svc_req *);
#define list_files ((rpc_uint)2)
extern  list_output * list_files_1(list_input *, CLIENT *);
extern  list_output * list_files_1_svc(list_input *, struct svc_req *);
#define delete_file ((rpc_uint)3)
extern  delete_output * delete_file_1(delete_input *, CLIENT *);
extern  delete_output * delete_file_1_svc(delete_input *, struct svc_req *);
#define write_file ((rpc_uint)4)
extern  write_output * write_file_1(write_input *, CLIENT *);
extern  write_output * write_file_1_svc(write_input *, struct svc_req *);
#define read_file ((rpc_uint)5)
extern  read_output * read_file_1(read_input *, CLIENT *);
extern  read_output * read_file_1_svc(read_input *, struct svc_req *);
#define copy_file ((rpc_uint)6)
extern  copy_output * copy_file_1(copy_input *, CLIENT *);
extern  copy_output * copy_file_1_svc(copy_input *, struct svc_req *);

#else /* Old Style C */
#define create_file ((rpc_uint)1)
extern  create_output * create_file_1();
extern  create_output * create_file_1_svc();
#define list_files ((rpc_uint)2)
extern  list_output * list_files_1();
extern  list_output * list_files_1_svc();
#define delete_file ((rpc_uint)3)
extern  delete_output * delete_file_1();
extern  delete_output * delete_file_1_svc();
#define write_file ((rpc_uint)4)
extern  write_output * write_file_1();
extern  write_output * write_file_1_svc();
#define read_file ((rpc_uint)5)
extern  read_output * read_file_1();
extern  read_output * read_file_1_svc();
#define copy_file ((rpc_uint)6)
extern  copy_output * copy_file_1();
extern  copy_output * copy_file_1_svc();
#endif /* Old Style C */

#endif /* !_SSNFS_H_RPCGEN */