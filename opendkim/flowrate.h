/*
**  Copyright (c) 2011, The OpenDKIM Project.  All rights reserved.
*/

#ifndef lint
static char flowrate_h_id[] = "@(#)$Id$";
#endif /* !lint */

#ifndef _FLOWRATE_H_
#define _FLOWRATE_H_

#include "build-config.h"

/* system includes */
#include <sys/param.h>
#include <sys/types.h>

/* opendkim includes */
#include "opendkim-db.h"

#ifdef __STDC__
# ifndef __P
#  define __P(x)  x
# endif /* ! __P */
#else /* __STDC__ */
# ifndef __P
#  define __P(x)  ()
# endif /* ! __P */
#endif /* __STDC__ */

/* prototypes */
extern int dkimf_rate_check __P((const char *, DKIMF_DB, DKIMF_DB, int,
                                 unsigned int *));

#endif /* _FLOWRATE_H_ */
