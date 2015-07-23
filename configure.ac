AC_PREREQ(2.65)
AC_INIT([mgui], [0.0.1], [tarik@ilixi.org], [ilixi_mgui], [http://www.ilixi.org])
AC_CONFIG_MACRO_DIR([build/m4])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11 -Wall foreign no-define])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CXX
AC_HEADER_STDC

AC_SEARCH_LIBS([uloop_init], [ubox], [], [AC_MSG_ERROR([unable to find the uloop_init function])])
AC_SEARCH_LIBS([ubus_connect], [ubus], [], [AC_MSG_ERROR([unable to find the ubus_connect function])])
AC_SEARCH_LIBS([uci_commit], [uci], [], [AC_MSG_ERROR([unable to find the uci_commit function])])
AC_SEARCH_LIBS([blobmsg_add_object], [blobmsg_json], [], [AC_MSG_ERROR([unable to find the blobmsg_get_u32 function])])

PKG_CHECK_MODULES([ILIXI], [ilixi >= 1.0.0])

MGUI_DATADIR=$datadir/ilixi-1.0.0
AC_SUBST(MGUI_DATADIR)

AC_CONFIG_FILES([
	Makefile \
	data/Makefile \
	data/images/Makefile \
	data/theme/Makefile \
	src/Makefile
])

AC_OUTPUT

AC_MSG_RESULT([
   $PACKAGE configuration summary
  -----------------------------
   Version                  : $VERSION
   Install prefix           : $prefix
   CPPFLAGS                 : $CPPFLAGS
   CFLAGS                   : $CFLAGS
   LDFLAGS                  : $LDFLAGS   

Type \`make' to build $PACKAGE.
Type \`make install' to install $PACKAGE.
]);
