AC_INIT([pg_inetx], [0.1], [astralblue@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_MACRO_DIRS([m4])
AX_LIB_POSTGRESQL
test -n "$POSTGRESQL_VERSION" || AC_MSG_ERROR([PostgreSQL is not found])
CC=`$PG_CONFIG --cc`
PG_INCLUDEDIR_SERVER=`$PG_CONFIG --includedir-server`
PG_PKGLIBDIR=`$PG_CONFIG --pkglibdir`
PG_SHAREDIR=`$PG_CONFIG --sharedir`
PG_CFLAGS=`$PG_CONFIG --cflags`
PG_CPPFLAGS=`$PG_CONFIG --cppflags`
PG_LDFLAGS=`$PG_CONFIG --ldflags | sed -e 's#^# #' -e 's#[[:space:]]-L\.\./[^[:space:]]*##'`
PG_LIBS=`$PG_CONFIG --libs`
AC_SUBST([PG_INCLUDEDIR_SERVER])
AC_SUBST([PG_PKGLIBDIR])
AC_SUBST([PG_SHAREDIR])
AC_SUBST([PG_CFLAGS])
AC_SUBST([PG_CPPFLAGS])
AC_SUBST([PG_LDFLAGS])
AC_SUBST([PG_LIBS])
AC_PROG_CC
AC_PROG_CC_C99
test "_$ac_cv_prog_cc_c99" != _no || AC_MSG_ERROR([compiler does not support C99])
AM_PROG_AR
AC_PROG_LIBTOOL
LT_INIT
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/inetx.control
		 src/inetx--1.0.sql])
AC_OUTPUT
