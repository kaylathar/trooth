AC_INIT(trooth, 0.1, kayla@kayla.is)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/bigint.c)

PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])

AM_MAINTAINER_MODE([disable])
AM_PROG_CC_C_O

AC_PROG_RANLIB
AM_INIT_AUTOMAKE

AC_OUTPUT(Makefile src/Makefile tests/Makefile include/Makefile)
