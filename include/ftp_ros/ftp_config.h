#define HAVE_UNISTD_H
#define HAVE_INTTYPES_H
#define HAVE_STDLIB_H
#define HAVE_SYS_SELECT_H
#define HAVE_SYS_SOCKET_H
#define HAVE_SYS_TIME_H
#define HAVE_ARPA_INET_H
#define HAVE_NETINET_IN_H
/* #undef HAVE_WINSOCK2_H */

/* Functions */
#define HAVE_STRCASECMP
/* #undef HAVE__STRICMP */
#define HAVE_SNPRINTF
/* #undef HAVE__SNPRINTF */

/* Workaround for platforms without POSIX strcasecmp (e.g. Windows) */
#ifndef HAVE_STRCASECMP
# ifdef HAVE__STRICMP
# define strcasecmp _stricmp
# define HAVE_STRCASECMP
# endif
#endif

/* Symbols */
#define HAVE___FUNC__
#define HAVE___FUNCTION__

/* Workaround for platforms without C90 __func__ */
#ifndef HAVE___FUNC__
# ifdef HAVE___FUNCTION__
# define __func__ __FUNCTION__
# define HAVE___FUNC__
# endif
#endif