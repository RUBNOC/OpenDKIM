/*
**  Copyright (c) 2005-2008 Sendmail, Inc. and its suppliers.
**    All rights reserved.
**
**  Copyright (c) 2009, 2010, The OpenDKIM Project.  All rights reserved.
*/

#ifndef _DKIM_TYPES_H_
#define _DKIM_TYPES_H_

#ifndef lint
static char dkim_types_h_id[] = "@(#)$Id: dkim-types.h,v 1.23 2010/10/28 02:41:25 cm-msk Exp $";
#endif /* !lint */

#include "build-config.h"

/* system includes */
#include <sys/types.h>
#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#endif /* HAVE_STDBOOL_H */
#ifdef USE_TRE
# ifdef TRE_PRE_080
#  include <tre/regex.h>
# else /* TRE_PRE_080 */
#  include <tre/tre.h>
# endif /* TRE_PRE_080 */
#else /* USE_TRE */
# include <regex.h>
#endif /* USE_TRE */

/* libar includes */
#ifdef USE_ARLIB
# include "ar.h"
#endif /* USE_ARLIB */

#ifdef USE_GNUTLS
# include <gnutls/gnutls.h>
# include <gnutls/crypto.h>
# include <gnutls/abstract.h>
#else /* USE_GNUTLS */
/* OpenSSL includes */
# include <openssl/pem.h>
# include <openssl/rsa.h>
# include <openssl/bio.h>
# include <openssl/err.h>
# include <openssl/sha.h>
#endif /* USE_GNUTLS */

#ifdef QUERY_CACHE
/* libdb includes */
# include <db.h>
#endif /* QUERY_CACHE */

/* libopendkim includes */
#include "dkim.h"
#include "dkim-internal.h"

/* struct dkim_pstate -- policy query state */
struct dkim_pstate
{
	unsigned int		ps_pflags;
	int			ps_qstatus;
	int			ps_state;
	dkim_policy_t		ps_policy;
	DKIM *			ps_dkim;
};

/* struct dkim_dstring -- a dynamically-sized string */
struct dkim_dstring
{
	int			ds_alloc;
	int			ds_max;
	int			ds_len;
	DKIM *			ds_dkim;
	unsigned char *		ds_buf;
};

/* struct dkim_header -- an RFC2822 header of some kind */
struct dkim_header
{
	int			hdr_flags;
	size_t			hdr_textlen;
	size_t			hdr_namelen;
	u_char *		hdr_text;
	u_char *		hdr_colon;
	struct dkim_header *	hdr_next;
};

/* hdr_flags bits */
#define	DKIM_HDR_SIGNED		0x01

/* struct dkim_plist -- a parameter/value pair */
struct dkim_plist
{
	u_char *		plist_param;
	u_char *		plist_value;
	struct dkim_plist *	plist_next;
};

/* struct dkim_set -- a set of parameter/value pairs */
struct dkim_set
{
	_Bool			set_bad;
	dkim_set_t		set_type;
	u_char *		set_data;
	void *			set_udata;
	struct dkim_plist *	set_plist[NPRINTABLE];
	struct dkim_set *	set_next;
};

/* struct dkim_siginfo -- signature information for use by the caller */
struct dkim_siginfo
{
	int			sig_dnssec_key;
	u_int			sig_flags;
	u_int			sig_error;
	u_int			sig_bh;
	u_int			sig_version;
	u_int			sig_hashtype;
	u_int			sig_keytype;
	u_int			sig_keybits;
	size_t			sig_siglen;
	size_t			sig_keylen;
	size_t			sig_b64keylen;
	dkim_query_t		sig_query;
	dkim_alg_t		sig_signalg;
	dkim_canon_t		sig_hdrcanonalg;
	dkim_canon_t		sig_bodycanonalg;
	unsigned long long	sig_timestamp;
	u_char *		sig_domain;
	u_char *		sig_selector;
	u_char *		sig_sig;
	u_char *		sig_key;
	u_char *		sig_b64key;
	void *			sig_context;
	void *			sig_signature;
	struct dkim_canon *	sig_hdrcanon;
	struct dkim_canon *	sig_bodycanon;
	struct dkim_set *	sig_taglist;
	struct dkim_set *	sig_keytaglist;
};

#ifdef USE_GNUTLS
/* struct dkim_sha -- stuff needed to do a sha hash */
struct dkim_sha
{
	int			sha_tmpfd;
	u_int			sha_outlen;
	gnutls_hash_hd_t	sha_hd;
	u_char *		sha_out;
};
#else /* USE_GNUTLS */
/* struct dkim_sha1 -- stuff needed to do a sha1 hash */
struct dkim_sha1
{
	int			sha1_tmpfd;
	BIO *			sha1_tmpbio;
	SHA_CTX			sha1_ctx;
	u_char			sha1_out[SHA_DIGEST_LENGTH];
};

# ifdef HAVE_SHA256
/* struct dkim_sha256 -- stuff needed to do a sha256 hash */
struct dkim_sha256
{
	int			sha256_tmpfd;
	BIO *			sha256_tmpbio;
	SHA256_CTX		sha256_ctx;
	u_char			sha256_out[SHA256_DIGEST_LENGTH];
};
# endif /* HAVE_SHA256 */
#endif /* USE_GNUTLS */

/* struct dkim_canon -- a canonicalization status handle */
struct dkim_canon
{
	_Bool			canon_done;
	_Bool			canon_hdr;
	_Bool			canon_blankline;
	int			canon_lastchar;
	int			canon_bodystate;
	u_int			canon_hashtype;
	u_int			canon_blanks;
	size_t			canon_hashbuflen;
	size_t			canon_hashbufsize;
	off_t			canon_remain;
	off_t			canon_wrote;
	off_t			canon_length;
	dkim_canon_t		canon_canon;
	u_char *		canon_hashbuf;
	u_char *		canon_hdrlist;
	void *			canon_hash;
	struct dkim_dstring *	canon_buf;
	struct dkim_header *	canon_sigheader;
	struct dkim_canon *	canon_next;
};

/* struct dkim_rsa -- stuff needed to do RSA sign/verify */
struct dkim_rsa
{
#ifdef USE_GNUTLS
	size_t			rsa_rsaoutlen;
	size_t			rsa_keysize;
	gnutls_x509_privkey_t	rsa_key;
	gnutls_pubkey_t		rsa_pubkey;
	gnutls_datum_t		rsa_sig;
	gnutls_datum_t		rsa_digest;
	gnutls_datum_t 		rsa_rsaout;
#else /* USE_GNUTLS */
	u_char			rsa_pad;
	size_t			rsa_keysize;
	size_t			rsa_rsainlen;
	size_t			rsa_rsaoutlen;
	EVP_PKEY *		rsa_pkey;
	RSA *			rsa_rsa;
	u_char *		rsa_rsain;
	u_char *		rsa_rsaout;
#endif /* USE_GNUTLS */
};

/* struct dkim_test_dns_data -- simulated DNS replies */
struct dkim_test_dns_data
{
	int			dns_class;
	int			dns_type;
	int			dns_prec;
	u_char *		dns_query;
	u_char *		dns_reply;
	struct dkim_test_dns_data * dns_next;
};

/* struct dkim_unbound_cb_data -- libunbound callback data */
struct dkim_unbound_cb_data
{
	int			ubd_done;
	int			ubd_rcode;
	int			ubd_id;
	int			ubd_type;
	u_int			ubd_result;
	DKIM_STAT		ubd_stat;
	size_t			ubd_buflen;
	u_char *		ubd_buf;
	const char *		ubd_jobid;
};

/* struct dkim -- a complete DKIM transaction context */
struct dkim
{
	_Bool			dkim_partial;
	_Bool			dkim_bodydone;
	_Bool			dkim_subdomain;
	_Bool			dkim_skipbody;
#ifdef _FFR_RESIGN
	_Bool			dkim_hdrbind;
#endif /* _FFR_RESIGN */
	_Bool			dkim_eoh_reentry;
	int			dkim_mode;
	int			dkim_state;
	int			dkim_chunkstate;
	int			dkim_chunksm;
	int			dkim_timeout;
	int			dkim_presult;
	int			dkim_hdrcnt;
	int			dkim_minsiglen;
#ifdef _FFR_RESIGN
	u_int			dkim_refcnt;
#endif /* _FFR_RESIGN */
#ifdef QUERY_CACHE
	u_int			dkim_cache_queries;
	u_int			dkim_cache_hits;
#endif /* QUERY_CACHE */
	u_int			dkim_version;
	u_int			dkim_sigcount;
	int			dkim_dnssec_policy;
	size_t			dkim_margin;
	size_t			dkim_b64siglen;
	size_t			dkim_keylen;
	size_t			dkim_errlen;
	time_t			dkim_timestamp;
#ifdef _FFR_PARSE_TIME
	time_t			dkim_msgdate;
#endif /* _FFR_PARSE_TIME */
	dkim_query_t		dkim_querymethod;
	dkim_canon_t		dkim_hdrcanonalg;
	dkim_canon_t		dkim_bodycanonalg;
	dkim_alg_t		dkim_signalg;
#ifdef _FFR_ATPS
	_Bool			dkim_atps;
#endif /* _FFR_ATPS */
	off_t			dkim_bodylen;
	off_t			dkim_signlen;
	const u_char *		dkim_id;
	u_char *		dkim_domain;
	u_char *		dkim_user;
	u_char *		dkim_selector;
	u_char *		dkim_b64key;
	u_char *		dkim_b64sig;
	u_char *		dkim_key;
	u_char *		dkim_reportaddr;
	u_char *		dkim_sender;
	u_char *		dkim_signer;
	u_char *		dkim_error;
	u_char *		dkim_hdrlist;
	u_char *		dkim_zdecode;
	u_char *		dkim_tmpdir;
	DKIM_SIGINFO *		dkim_signature;
	void *			dkim_closure;
	const void *		dkim_user_context;
#ifdef _FFR_RESIGN
	DKIM *			dkim_resign;
#endif /* _FFR_RESIGN */
	struct dkim_siginfo **	dkim_siglist;
	struct dkim_set *	dkim_sethead;
	struct dkim_set *	dkim_settail;
	struct dkim_set *	dkim_sigset;
	struct dkim_header *	dkim_hhead;
	struct dkim_header *	dkim_htail;
	struct dkim_header *	dkim_senderhdr;
	struct dkim_canon *	dkim_canonhead;
	struct dkim_canon *	dkim_canontail;
	struct dkim_dstring *	dkim_hdrbuf;
	struct dkim_dstring *	dkim_canonbuf;
	struct dkim_test_dns_data * dkim_dnstesth;
	struct dkim_test_dns_data * dkim_dnstestt;
	DKIM_LIB *		dkim_libhandle;
};

/* struct dkim_lib -- a DKIM library context */
struct dkim_lib
{
	_Bool			dkiml_signre;
	_Bool			dkiml_skipre;
	u_int			dkiml_flags;
	u_int			dkiml_timeout;
	u_int			dkiml_version;
	u_int			dkiml_callback_int;
	u_int			dkiml_flsize;
	u_int			dkiml_nalwayshdrs;
	time_t			dkiml_fixedtime;
	unsigned long		dkiml_sigttl;
	unsigned long		dkiml_clockdrift;
	dkim_query_t		dkiml_querymethod;
	u_int *			dkiml_flist;
	void *			(*dkiml_malloc) (void *closure, size_t nbytes);
	void			(*dkiml_free) (void *closure, void *p);
	u_char **		dkiml_senderhdrs;
	u_char **		dkiml_alwayshdrs;
	u_char **		dkiml_oversignhdrs;
	u_char **		dkiml_mbs;
#ifdef QUERY_CACHE
	DB *			dkiml_cache;
#endif /* QUERY_CACHE */
	regex_t			dkiml_hdrre;
	regex_t			dkiml_skiphdrre;
	DKIM_CBSTAT		(*dkiml_key_lookup) (DKIM *dkim,
				                     DKIM_SIGINFO *sig,
				                     u_char *buf,
				                     size_t buflen);
	DKIM_CBSTAT		(*dkiml_policy_lookup) (DKIM *dkim,
				                        u_char *query,
				                        _Bool excheck,
				                        u_char *buf,
				                        size_t buflen,
				                        int *qstat);
	void *			(*dkiml_sig_handle) (void *closure);
	void			(*dkiml_sig_handle_free) (void *closure,
				                          void *user);
	void			(*dkiml_sig_tagvalues) (void *user,
				                        dkim_param_t pcode,
				                        const u_char *param,
				                        const u_char *value);
	DKIM_CBSTAT		(*dkiml_prescreen) (DKIM *dkim,
				                    DKIM_SIGINFO **sigs,
				                    int nsigs);
	DKIM_CBSTAT		(*dkiml_final) (DKIM *dkim,
				                DKIM_SIGINFO **sigs,
				                int nsigs);
	void			(*dkiml_dns_callback) (const void *context);
	void			*dkiml_dns_service;
	int			(*dkiml_dns_start) (void *srv, int type,
				                    unsigned char *query,
				                    unsigned char *buf,
				                    size_t buflen,
				                    void **qh);
	int			(*dkiml_dns_cancel) (void *srv, void *qh);
	int			(*dkiml_dns_waitreply) (void *srv,
				                        void *qh,
				                        struct timeval *to,
				                        size_t *bytes,
				                        int *error,
				                        int *dnssec);
	u_char			dkiml_tmpdir[MAXPATHLEN + 1];
	u_char			dkiml_queryinfo[MAXPATHLEN + 1];
};

#endif /* _DKIM_TYPES_H_ */
