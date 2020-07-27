AC_PREREQ([2.69])
AC_INIT([smart-records], [0.1], [])
AM_PROG_LIBTOOL
AC_CONFIG_SRCDIR([src])
AC_CONFIG_MACRO_DIRS([m4])
AC_ENABLE_SHARED

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

LT_INIT
AM_INIT_AUTOMAKE

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset socket strchr strdup strerror strndup])

AC_ARG_ENABLE(interactive_terminal,
    [AS_HELP_STRING([--with-interactive-terminal], [Compile with interactive terminal support for password prompting.], )],
    [interactive_terminal=$withval], [interactive_terminal=no])
if test x"$interactive_terminal" == x"yes"
then
    AC_DEFINE([_WITH_INTERACTIVE_TERMINAL], [1], ["Defined if you have interactive terminal"])
    CFLAGS="$CFLAGS -D_WITH_INTERACTIVE_TEMRINAL"
fi



AC_ARG_ENABLE(csv,
    [AS_HELP_STRING([--enable-csv], [Compile with CSV support], )],
    [csv=$enableval], [csv=yes]) 
if test x"$csv" == x"yes"
then
    AC_DEFINE([_HAS_CSV], [1], ["Defined if you have UDP support"])
    CFLAGS="$CFLAGS -D_HAS_CSV"
fi

AC_ARG_ENABLE(udp,
    [AS_HELP_STRING([--enable-udp], [Compile with CSV support], )],
    [udp=$enableval], [udp=yes]) 
if test x"$udp" == x"yes"
then
    AC_DEFINE([HAS_UDP], [1], ["Defined if you have UDP support"])
    CFLAGS="$CFLAGS -D_HAS_UDP"
fi

AC_ARG_WITH(mysql,
    [AS_HELP_STRING[(--with-mysql), [Compile with MySQL backend support, require libmysqlclient]]],
    [],
    [with_mysql=yes])

echo $with_mysql
if test x"with_mysql" != x"no"
then
    PKG_CHECK_MODULES(MYSQL, mysqlclient, [mysql=yes], [mysql=no])
    if test x"mysql" = x"no"
    then
        AC_MSG_FAILURE([libmysqlclient not found ! (--without-mysql to disable MySQL support)])
    else
        CFLAGS="$CFLAGS $MYSQL_CFLAGS"
        LDFLAGS="$LDFLAGS $MYSQL_LDFLAGS"
    fi
fi


AM_CONDITIONAL([HAS_CSV], [test x"csv" != x"no"] )
AM_CONDITIONAL([HAS_MYSQL], [test x"mysql" != x"no"] )
AM_CONDITIONAL([HAS_UDP], [test x"udp" != x"no"] )


AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/csv_backend/Makefile
                 src/mysql_backend/Makefile
                 src/udp_backend/Makefile])
AC_OUTPUT
