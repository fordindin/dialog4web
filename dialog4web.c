/*-
 * Copyright (c) 2014 Denis Barov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include "dialog4web.h"

const char* greatingpage="<html><body><h1>Welcome, %s!</center></h1></body></html>";

struct connection_info_struct
{
		int connectiontype;
		char *answerstring;
		struct MHD_PostProcessor *postprocessor;
};

const int maxdocsize = 262144;
bool stop_flag = false;

static StringList *
parse_env_sl(char const *env_name)
{
	StringList *sl;
	char *env;
	char *temp;
	char *token;

	sl = sl_init();
	env = getenv(env_name);
	if (env != NULL) {
		temp = strdup(env);
		while ((token = strsep(&temp, " \t")) != NULL) {
			if (token[0] == '\0')
				continue;
			sl_add(sl, strdup(token));
		}
		free(temp);
	}
	return (sl);
}

static int
iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
				const char *filename, const char *content_type,
				const char *transfer_encoding, const char *data,
				uint64_t off, size_t size)
{
		struct connection_info_struct *con_info = coninfo_cls;
		if (0 == strcmp (key, "name"))
		{
				if ((size > 0) && (size <= MAXNAMESIZE))
				{
						char *answerstring;
						answerstring = malloc (MAXANSWERSIZE);
						if (!answerstring) return MHD_NO;
						snprintf (answerstring, MAXANSWERSIZE, greatingpage, data);
						con_info->answerstring = answerstring;
				}
				else con_info->answerstring = NULL;
				return MHD_NO;
		}
		return MHD_YES;
}


void request_completed (void *cls, struct MHD_Connection *connection,
				void **con_cls,
				enum MHD_RequestTerminationCode toe)
{
		struct connection_info_struct *con_info = *con_cls;
		if (NULL == con_info) return;
		if (con_info->connectiontype == POST)
		{
				MHD_destroy_post_processor (con_info->postprocessor);
				if (con_info->answerstring) free (con_info->answerstring);
		}
		free (con_info);
		*con_cls = NULL;
}

static int
answer_to_connection (
				void *cls,
				struct MHD_Connection *connection,
				const char *url,
				const char *method,
				const char *version,
				const char *upload_data,
				size_t *upload_data_size,
				void **con_cls
				)
{
		int ret;
		char *buf = malloc(maxdocsize);
		char *body = malloc(maxdocsize);
		struct MHD_Response *response;

		if(NULL == *con_cls)
		{
				struct connection_info_struct *con_info;
				con_info = malloc (sizeof (struct connection_info_struct));
				if (NULL == con_info) return MHD_NO;
				con_info->answerstring = NULL;
		}

		if ( 0 == strcmp(method, "GET")){
				ItemGroupList grouplist;
				InputItem **items;

				grouplist = get_group_list();
				buf = GroupListStr(grouplist);
				sprintf(body, bodytemplate, buf);

			response = MHD_create_response_from_buffer (strlen (body), (void *) body,
				 MHD_RESPMEM_PERSISTENT);
			MHD_add_response_header (response, "Content-Type", MIMETYPE);
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			free(body);
			free(buf);
		}  else if ( 0 == strcmp(method, "POST")){
			body = "{succes: true}";
			con_info->postprocessor = MHD_create_post_processor (
							connection,
							POSTBUFFERSIZE,
							iterate_post,
							(void*) con_info
							);
			response = MHD_create_response_from_buffer (strlen (body), (void *) body,
				 MHD_RESPMEM_PERSISTENT);
			MHD_add_response_header (response, "Content-Type", "Application/JSON");
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			stop_flag = true;
		}
  return ret;
}

int
main ()
{
		struct MHD_Daemon *daemon;

		daemon = MHD_start_daemon ( MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
								&answer_to_connection, NULL, MHD_OPTION_NOTIFY_COMPLETED,
								&request_completed, NULL, MHD_OPTION_END);

  if (NULL == daemon)
    return 1;

	while ( !stop_flag) { };
	/* Allow server send final success response */
	sleep(1);
  MHD_stop_daemon (daemon);
  return 0;
}
