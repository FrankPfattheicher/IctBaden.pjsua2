
#include <pjsua.h>
#include <pjsua-lib/pjsua_internal.h>
#include "pj-media-neg.h"

#define THIS_FILE "pj_media_neg.cpp"

extern "C"


/* Syntax error handler for parser. */
static void on_syntax_error(pj_scanner * scanner)
{
	PJ_UNUSED_ARG(scanner);
	PJ_LOG(4, (THIS_FILE, "Scanner syntax error at %s", scanner->curptr));
	PJ_THROW(PJ_EINVAL);
}


//Helper function to find a specific string using the scanner.
//It returns the pointer to where the string is found and the number of chars parsed (i.e. not the length of the string)
void scanner_find_string(pj_scanner* scanner, char* wanted, pj_str_t* result)
{
	pj_scan_state state;
	pj_scan_save_state(scanner, &state);
	do {
		pj_scan_get_until_ch(scanner, wanted[0], result);
		if (pj_scan_strcmp(scanner, wanted, (int)strlen(wanted)) == 0) {
			//Found wanted string, update real scanner
			unsigned steps_to_advance = (unsigned)(scanner->curptr - state.curptr);
			pj_scan_restore_state(scanner, &state);
			pj_scan_advance_n(scanner, steps_to_advance, PJ_FALSE);
			result->ptr = scanner->curptr;
			result->slen = steps_to_advance;
			break;
		}
		else {
			pj_scan_advance_n(scanner, 1, PJ_FALSE);
		}

		if (pj_scan_is_eof(scanner)) {
			PJ_LOG(4, (THIS_FILE, "Scanner EOF"));
			break;
		}

	} while (1);
}

static int calculate_new_content_length(char* buffer)
{
	char* body_delim = (char*)"\r\n\r\n"; //Sip message body starts with a newline and final empty line after sdp does not count
	char* body_start = strstr(buffer, body_delim);
	if (body_start != NULL) {
		body_start = body_start + strlen(body_delim);
		return (int)strlen(body_start);
	}
	else {
		PJ_LOG(1, (THIS_FILE,
			"Error: Could not find Content-Length header. The correct length can not be set. This must never happen."));
		return -1;
	}
}

static int update_content_length(char* buffer, pjsip_msg* msg)
{
#define CONTENT_LEN_BUF_SIZE 10
	PJ_USE_EXCEPTION;
	pj_scanner scanner;
	pj_str_t result = { NULL, 0 };
	pj_str_t current_content_len;
	char* content_length = (char*)"Content-Length";
	char new_content_len_buf[CONTENT_LEN_BUF_SIZE];
	int new_content_len = calculate_new_content_length(buffer);
	if (new_content_len == -1) {
		return -1;
	}

	//Step 1: Update the pjsip_msg to reflect the new body size (for incoming messages only)
	if (msg != NULL) {
		msg->body->len = (int)new_content_len;
	}

	//Step 2: Rewrite the buffer so the Content-Length header is correct
	pj_scan_init(&scanner, buffer, strlen(buffer), 0, &on_syntax_error);

	pj_ansi_snprintf(new_content_len_buf, CONTENT_LEN_BUF_SIZE, "%d", new_content_len);

	PJ_TRY{
		scanner_find_string(&scanner, content_length, &result);

		pj_scan_get_until_chr(&scanner, "0123456789", &result);

		pj_scan_get_until_ch(&scanner, '\r', &result);
		pj_strset(&current_content_len, result.ptr, result.slen);
		PJ_LOG(4, (THIS_FILE, "Current Content-Length is: %.*s and new Content-Length is %d .", current_content_len.slen, current_content_len.ptr, new_content_len));
		if ((pj_ssize_t)strlen(new_content_len_buf) <= current_content_len.slen)
		{
			int len_offset = (int)(result.ptr - scanner.begin);
			PJ_LOG(4, (THIS_FILE, "Updated content length needs the same or less bytes, no need to do buffer copy"));
			//even though scanner and buffer are in different memory locations the content is identical
			pj_memset(buffer + len_offset, ' ', result.slen);
			pj_memcpy(buffer + len_offset, new_content_len_buf, strlen(new_content_len_buf));
		}
		 else {
		  PJ_TODO(SUPPORT_GROWING_THE_BUFFER);
		  PJ_LOG(4, (THIS_FILE, "Updated content length needs more bytes than old one, we must do expand and copy. TODO"));
		}
	} PJ_CATCH_ANY{
		PJ_LOG(4, (THIS_FILE, "Exception thrown when searching for Content-Length, incorrect value will be used. Must never happen."));
	}
	PJ_END;
	pj_scan_fini(&scanner);
	return new_content_len;
}


//For incoming INVITE and 200 OK

#define MAX_LINE_COUNT  60
#define MAX_LINE_LENGTH 120

static void fix_telephone_event_negotiation(pjsip_rx_data* rdata)
{
	char* org_buffer = rdata->msg_info.msg_buf;
	char new_buffer[PJSIP_MAX_PKT_LEN];

	pj_bzero(new_buffer, PJSIP_MAX_PKT_LEN);

	PJ_LOG(4, (THIS_FILE, "**********Incoming INVITE or 200 with telephone-event*************"));
	PJ_LOG(4, (THIS_FILE, "%s", org_buffer));
	PJ_LOG(4, (THIS_FILE, "***************************************************************"));

	char line[MAX_LINE_COUNT][MAX_LINE_LENGTH];
	strcpy(new_buffer, org_buffer);

	int sx = 0;
	char* pch = strtok(new_buffer, "\n");
	while (pch != NULL && sx < MAX_LINE_COUNT)
	{
		pj_bzero(line[sx], MAX_LINE_LENGTH);
		strncpy(line[sx], pch, MAX_LINE_LENGTH - 1);
		sx++;
		pch = strtok(NULL, "\n");
	}

	// Collect present telephone-event lines only (position + clock rate).
	static const struct { const char* needle; int freq; } rates[] = {
		{ "telephone-event/8000", 8000 },
		{ "telephone-event/16000", 16000 },
		{ "telephone-event/32000", 32000 },
		{ "telephone-event/44100", 44100 },
		{ "telephone-event/48000", 48000 },
	};
	const int rate_count = (int)(sizeof(rates) / sizeof(rates[0]));

	struct te_entry {
		int pos;
		int freq;
		int line_idx;
	};
	te_entry entries[5];
	int n = 0;

	for (int dx = 0; dx < sx; dx++)
	{
		for (int r = 0; r < rate_count; r++)
		{
			if (strstr(line[dx], rates[r].needle) != NULL)
			{
				if (n < rate_count)
				{
					entries[n].pos = dx;
					entries[n].freq = rates[r].freq;
					entries[n].line_idx = dx;
					n++;
				}
				break;
			}
		}
	}

	if (n <= 1)
		return;

	// Slot order = ascending line positions; content order = ascending frequency.
	int positions[5];
	int by_freq[5];
	for (int i = 0; i < n; i++)
	{
		positions[i] = entries[i].pos;
		by_freq[i] = i;
	}
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (positions[j] < positions[i])
			{
				int t = positions[i];
				positions[i] = positions[j];
				positions[j] = t;
			}
			if (entries[by_freq[j]].freq < entries[by_freq[i]].freq)
			{
				int t = by_freq[i];
				by_freq[i] = by_freq[j];
				by_freq[j] = t;
			}
		}
	}

	int remap[MAX_LINE_COUNT];
	for (int i = 0; i < sx; i++)
		remap[i] = i;
	for (int i = 0; i < n; i++)
		remap[positions[i]] = entries[by_freq[i]].line_idx;

	pj_bzero(new_buffer, PJSIP_MAX_PKT_LEN);
	for (int dx = 0; dx < sx; dx++)
	{
		strcat(new_buffer, line[remap[dx]]);
		strcat(new_buffer, "\n");
	}

	strcpy(org_buffer, new_buffer);
}


static pj_status_t media_neg_mod_on_rx(pjsip_rx_data* rdata)
{
	pjsip_cseq_hdr* cseq = rdata->msg_info.cseq;
	PJ_LOG(4, (THIS_FILE, "media_neg_mod_on_rx"));

	if (cseq != NULL && cseq->method.id == PJSIP_INVITE_METHOD) {
		PJ_LOG(4, (THIS_FILE, "Incoming INVITE or 200 OK. Sort telephone-event lines by ascending clock rate if multiple are present"));

		fix_telephone_event_negotiation(rdata);
	}

	return PJ_FALSE;
}

/* module for sdp negotiation info.*/
static pjsip_module media_neg_module = {
	NULL, NULL,                     /* prev, next.      */
	{ (char*)"mneg_ext", 8 },       /* Name.            */
	-1,                             /* Id               */
	0,                              /* Priority         */
	NULL,                           /* load()           */
	NULL,                           /* start()          */
	NULL,                           /* stop()           */
	NULL,                           /* unload()         */
	&media_neg_mod_on_rx,           /* on_rx_request()  */
	NULL,                           /* on_rx_response() */
	NULL,                           /* on_tx_request()  */
	NULL,                           /* on_tx_response() */
	NULL,                           /* on_tsx_state()   */
};

extern "C"
{

	PJ_EXPORT_DECL_SPECIFIER pj_status_t pj_enable_media_negotiation_module()
	{
		OutputDebugStringA((char*)"pj_enable_media_negotiation_module");
		return pjsip_endpt_register_module(pjsua_get_pjsip_endpt(), &media_neg_module);
	}

}