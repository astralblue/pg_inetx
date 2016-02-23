AM_CFLAGS = -Wall -Wextra -Werror
AM_LDFLAGS = -module

pglibdir = @PG_PKGLIBDIR@
pglib_LTLIBRARIES = inetx.la

inetx_la_CFLAGS = @PG_CFLAGS@ $(AM_CFLAGS)
inetx_la_CPPFLAGS = -I @PG_INCLUDEDIR_SERVER@ @PG_CPPFLAGS@
inetx_la_LDFLAGS = @PG_LDFLAGS@ $(AM_LDFLAGS)
#inetx_la_LDADD = @PG_LIBS@

pgextdir = @PG_SHAREDIR@/extension
pgext_DATA = inetx.control inetx--1.0.sql
