# Process this file with autoconf to produce a configure script.
#
# Configure.in for RDieHarder
# Copyright (C) 2008 - 2022  Dirk Eddelbuettel and licensed under GNU GPL

# The version set here will propagate to other files from here
AC_INIT([RDieHarder],[0.2.4])

# Ensure C++ is set up as R expects
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
    AC_MSG_ERROR([Could not determine R_HOME.])
fi

AC_MSG_CHECKING([endianness])
ENDIAN=`"${R_HOME}/bin/Rscript" -e 'cat(.Platform$endian)'`
if test "${ENDIAN}" == "little"; then
   AC_MSG_RESULT([little])
   endianness=1
else
   AC_MSG_RESULT([big])
   endianness=0
fi

# Checks for common programs using default macros
AC_PROG_CC

# Check for non-standard programs: gsl-config(1)
AC_PATH_PROG([GSL_CONFIG], [gsl-config])

# If gsl-config was found, let's use it
if test "${GSL_CONFIG}" != ""; then
    # Use gsl-config for header and linker arguments
    GSL_CFLAGS=`${GSL_CONFIG} --cflags`
    GSL_LIBS=`${GSL_CONFIG} --libs`
else
    AC_MSG_ERROR([gsl-config not found, is GSL installed?])
fi

dnl # offer configure argument support for dieharder includes
dnl AC_ARG_WITH([dieharder-include],
dnl             AS_HELP_STRING([--with-dieharder-include=INCLUDE_PATH],[dnl                            Supply the location of DieHarder header files]),
dnl             [dh_include_path=$withval],
dnl             [dh_include_path=""])

dnl if test [ -n "$dh_include_path" ] ; then
dnl     DH_INCDIR="$dh_include_path"
dnl else
dnl     # Now search for dieharder.h
dnl     DH_INCDIR=""
dnl     DH_TARGET=""
dnl     AC_MSG_CHECKING([DieHarder header files])
dnl     for dir in \
dnl         /usr/include \
dnl 	/usr/include/dieharder \
dnl 	/usr/local/include \
dnl 	/usr/local/include/dieharder \
dnl 	/usr/local/dieharder/include \
dnl 	/opt/include \
dnl 	/opt/include/dieharder \
dnl 	/opt/dieharder/include
dnl     do
dnl         if test -f ${dir}/libdieharder.h
dnl 	then
dnl 	    DH_INCDIR=${dir}
dnl 	    break
dnl 	fi
dnl     done

dnl     if [ test -n "${DH_INCDIR}" ] ; then
dnl         AC_MSG_RESULT([found])
dnl         # Test for sanity by looking for libdieharder.h and libdieharder.so,
dnl         # no explicit action on found, error on failure
dnl         AC_CHECK_FILE(["${DH_INCDIR}/libdieharder.h"],
dnl             ,
dnl             AC_MSG_ERROR([File libdieharder.h not in ${DH_INCDIR}.]))
dnl     else
dnl         AC_MSG_RESULT([need to build library])
dnl         if test -f src/libdieharder/dieharder/libdieharder.h; then
DH_TARGET="libdieharder.a"
DH_INCDIR="libdieharder"
DH_LIBDIR="libdieharder"
dnl         fi
dnl     fi
dnl fi


dnl # offer configure argument support for dieharder library
dnl AC_ARG_WITH([dieharder-libdir],
dnl             AS_HELP_STRING([--with-dieharder-libdir=LIBRARY_PATH],[dnl                            Supply the location of DieHarder library file]),
dnl             [dh_lib_path=$withval],
dnl             [dh_lib_path=""])

dnl if test [ -n "$dh_lib_path" ] ; then
dnl     DH_LIBDIR="$dh_lib_path"
dnl else
dnl     libfound=""
dnl     # Now search for libdieharder.so
dnl     AC_MSG_CHECKING([DieHarder library file])
dnl     for dir in \
dnl 	/usr/lib \
dnl 	/usr/lib/dieharder \
dnl 	/usr/local/lib \
dnl 	/usr/local/lib/dieharder \
dnl 	/usr/local/dieharder/lib \
dnl 	/opt/include \
dnl 	/opt/lib/dieharder \
dnl 	/opt/dieharder/lib
dnl     do
dnl         if test -f ${dir}/libdieharder.so; then
dnl 	    DH_LIBDIR=${dir}
dnl             libfound=${dir}
dnl 	    break
dnl 	fi
dnl     done
dnl     if test [ -n "$libfound" ] ; then
dnl         AC_MSG_RESULT([found])
dnl     else
dnl         AC_MSG_RESULT([none found])
dnl     fi
dnl fi

# Test for sanity by looking for libdieharder.so,
# no explicit action on found, error on failure
#AC_CHECK_FILE(["${DH_LIBDIR}/libdieharder.so"],
#	,
#	AC_MSG_ERROR([Library libdieharder.so not in ${DH_LIBDIR}.]))

# Expand into arguments
DH_CFLAGS="-I${DH_INCDIR} -I. -DRDIEHARDER"
#DH_LIBS="-L${DH_LIBDIR} -ldieharder"
DH_LIBS=
#DH_TGT="${DH_TARGET}"
DH_TGT=
ENDIANNESS="${endianness}"

# Now substitute these variables in src/Makevars.in to create src/Makevars
AC_SUBST(GSL_CFLAGS)
AC_SUBST(GSL_LIBS)
AC_SUBST(DH_CFLAGS)
AC_SUBST(DH_LIBS)
AC_SUBST(DH_TGT)
AC_SUBST(ENDIANNESS)
AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
#AC_CONFIG_FILES([src/libdieharder/Makefile])
AC_OUTPUT

#echo "
#${PACKAGE_NAME} ${PACKAGE_VERSION}
#================
#
#compiler flags:  ${DH_CFLAGS} ${GSL_CFLAGS}
#link argument:   ${DH_LIBS} ${GSL_LIBS}
#build dieharder: ${DH_TGT}
#"
