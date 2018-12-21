# Process this file with autoconf to produce a configure script.
#
# Configure.in for RDieHarder
# Copyright (C) 2008 Dirk Eddelbuettel and licensed under GNU GPL

# The version set here will propagate to other files from here
AC_INIT(RDieHarder, 0.1.5)

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

# offer configure argument support for dieharder includes
AC_ARG_WITH([dieharder-include],
            AC_HELP_STRING([--with-dieharder-include=INCLUDE_PATH],
                           [Supply the location of DieHarder header files]),
            [dh_include_path=$withval],
            [dh_include_path=""])

if test [ -n "$dh_include_path" ] ; then
    DH_INCDIR="$dh_include_path"
else
    # Now search for dieharder.h
    DH_INCDIR=""
    DH_TARGET=""
    AC_MSG_CHECKING([DieHarder header files])
    for dir in \
        /usr/include \
	/usr/include/dieharder \
	/usr/local/include \
	/usr/local/include/dieharder \
	/usr/local/dieharder/include \
	/opt/include \
	/opt/include/dieharder \ 
	/opt/dieharder/include 
    do
        if test -f ${dir}/libdieharder.h
	then 
	    DH_INCDIR=${dir}
	    break
	fi
    done

    if [ test -n "${DH_INCDIR}" ] ; then
        AC_MSG_RESULT([found])
        # Test for sanity by looking for libdieharder.h and libdieharder.so, 
        # no explicit action on found, error on failure
        AC_CHECK_FILE(["${DH_INCDIR}/libdieharder.h"],
            ,
            AC_MSG_ERROR([File libdieharder.h not in ${DH_INCDIR}.]))
    else
        AC_MSG_RESULT([need to build library])
        if test -f src/libdieharder/dieharder/libdieharder.h; then
            DH_TARGET="libdieharder.a"
            DH_INCDIR="libdieharder"
            DH_LIBDIR="libdieharder"
        fi
    fi
fi


# offer configure argument support for dieharder library
AC_ARG_WITH([dieharder-libdir],
            AC_HELP_STRING([--with-dieharder-libdir=LIBRARY_PATH],
                           [Supply the location of DieHarder library file]),
            [dh_lib_path=$withval],
            [dh_lib_path=""])

if test [ -n "$dh_lib_path" ] ; then
    DH_LIBDIR="$dh_lib_path"
else
    libfound=""
    # Now search for libdieharder.so
    AC_MSG_CHECKING([DieHarder library file])
    for dir in \
	/usr/lib \
	/usr/lib/dieharder \
	/usr/local/lib \
	/usr/local/lib/dieharder \
	/usr/local/dieharder/lib \
	/opt/include \
	/opt/lib/dieharder \ 
	/opt/dieharder/lib
    do
        if test -f ${dir}/libdieharder.so; then 
	    DH_LIBDIR=${dir}
            libfound=${dir}
	    break
	fi
    done
    if test [ -n "$libfound" ] ; then
        AC_MSG_RESULT([found])
    else
        AC_MSG_RESULT([none found])
    fi
fi

# Test for sanity by looking for libdieharder.so, 
# no explicit action on found, error on failure
#AC_CHECK_FILE(["${DH_LIBDIR}/libdieharder.so"],
#	,
#	AC_MSG_ERROR([Library libdieharder.so not in ${DH_LIBDIR}.]))

# Expand into arguments
DH_CFLAGS="-I${DH_INCDIR} -I. -DRDIEHARDER"
DH_LIBS="-L${DH_LIBDIR} -ldieharder"
DH_TGT="${DH_TARGET}"

# Now substitute these variables in src/Makevars.in to create src/Makevars
AC_SUBST(GSL_CFLAGS)
AC_SUBST(GSL_LIBS)
AC_SUBST(DH_CFLAGS)
AC_SUBST(DH_LIBS)
AC_SUBST(DH_TGT)
AC_OUTPUT(src/Makevars)
AC_OUTPUT(src/libdieharder/Makefile)

echo "
${PACKAGE_NAME} ${PACKAGE_VERSION}
================

compiler flags:  ${DH_CFLAGS} ${GSL_CFLAGS}
link argument:   ${DH_LIBS} ${GSL_LIBS}
build dieharder: ${DH_TGT}
"
