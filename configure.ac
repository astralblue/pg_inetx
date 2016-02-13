AC_INIT([pg_inet], [0.1], [astralblue@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_MACRO_DIRS([m4])
AX_LIB_POSTGRESQL
test -n "$POSTGRESQL_VERSION" || AC_MSG_ERROR([PostgreSQL is not found])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
