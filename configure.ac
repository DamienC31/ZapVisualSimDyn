AC_INIT(src/freereign.h)
#
# Making releases:
#   FR_MICRO_VERSION += 2;
#   FR_INTERFACE_AGE += 0;
#   FR_BINARY_AGE += 0;
# if any functions have been added, set FR_INTERFACE_AGE to 0.
# if backwards compatibility has been broken,
# set FR_BINARY_AGE and FR_INTERFACE_AGE to 0.
#
FR_MAJOR_VERSION=0
FR_MINOR_VERSION=2
FR_MICRO_VERSION=1
FR_INTERFACE_AGE=0
FR_BINARY_AGE=0
FR_VERSION=$FR_MAJOR_VERSION.$FR_MINOR_VERSION.$FR_MICRO_VERSION

AC_SUBST(FR_MAJOR_VERSION)
AC_SUBST(FR_MINOR_VERSION)
AC_SUBST(FR_MICRO_VERSION)
AC_SUBST(FR_INTERFACE_AGE)
AC_SUBST(FR_BINARY_AGE)
AC_SUBST(FR_VERSION)


echo "----------------------------"
echo "--Free Reign Version $FR_VERSION"
echo "--------"


dnl Checks for programs
AC_PROG_MAKE_SET
AC_ISC_POSIX
AC_PROG_INSTALL
AC_PROG_LN_S

AM_INIT_AUTOMAKE(freereign,[$FR_VERSION])
AM_MAINTAINER_MODE

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AM_SANITY_CHECK



dnl Switch to C++...
AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_CHECK_LIB(stdc++, getc,,
  AC_MSG_ERROR(You need to have libstdc++ installed))
AC_CHECK_HEADER(vector,,
  AC_MSG_ERROR(You need to have the libstdc++ headers installed))

AC_CHECK_HEADER(string,,
  AC_MSG_ERROR(You need to have a c++ compiler that supports the string class.))

AC_LANG_RESTORE

dnl X Checks..
AC_PATH_X
AC_PATH_XTRA

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST


AC_CHECK_HEADER(stdio.h)
AC_CHECK_HEADER(stdlib.h)
AC_HEADER_STDC


################################################################## XLib

saved_CFLAGS="$CFLAGS"
saved_LDFLAGS="$LDFLAGS"

AC_PATH_X
AC_PATH_XTRA

CFLAGS="$CFLAGS $X_CFLAGS"
X_LIBS="$X_LIBS $X_EXTRA_LIBS"

if test "x$no_x" = "xyes"; then
  AC_MSG_ERROR([
*** X libraries or include files not found. Check 'config.log' for
*** more details.])
fi

# check X11 library
AC_CHECK_LIB(X11, XOpenDisplay, X_LIBS="$X_LIBS -lX11",
  AC_MSG_ERROR([*** libX11 not found. Check 'config.log' for more detail.]),
  $X_LIBS)

# check for Xext library
AC_CHECK_LIB(Xext, XShmAttach, X_LIBS="$X_LIBS -lXext",
  # On AIX, it is in XextSam instead, but we still need -lXext
  AC_CHECK_LIB(XextSam, XShmAttach, X_LIBS="$X_LIBS -lXextSam -lXext",
    no_xext_lib=yes, $X_LIBS), $X_LIBS)

# check for ICE library
AC_CHECK_LIB(ICE, IceSetIOErrorHandler,X_LIBS="$X_LIBS -lICE",, $X_LIBS)

# check for SM library
AC_CHECK_LIB(SM, SmcClientID,X_LIBS="$X_LIBS -lSM",, $X_LIBS)

# check for Xt library
AC_CHECK_LIB(Xt, XtDisplayToApplicationContext,X_LIBS="$X_LIBS -lXt",,$X_LIBS)

# check for Xmu library
AC_CHECK_LIB(Xmu, XmuLookupStandardColormap, X_LIBS="$X_LIBS -lXmu",
  AC_MSG_ERROR([*** libXmu not found. Check 'config.log' for more detail.]),
  $X_LIBS)

# check for Xi library
AC_CHECK_LIB(Xi, XOpenDevice, X_LIBS="$X_LIBS -lXi",, $X_LIBS)

AC_SUBST(X_CFLAGS)
AC_SUBST(X_LDFLAGS)
AC_SUBST(X_LIBS)

CFLAGS=$saved_CFLAGS
LDFLAGS=$saved_LDFLAGS
################################################



################################################################## OpenGL

AC_CHECK_LIB(m, floor)

saved_CFLAGS="$CFLAGS"
saved_LDFLAGS="$LDFLAGS"
saved_LIBS="$LIBS"

AC_ARG_WITH(gl, [  --with-gl=DIR           specify OpenGL/Mesa prefix],
  gl_prefix=$with_gl)

if test x$gl_prefix != x ; then
  GL_CFLAGS="-I$gl_prefix/include"
  GL_LDFLAGS="-L$gl_prefix/lib"
fi

CFLAGS="$CFLAGS $X_CFLAGS $GL_CFLAGS"
LDFLAGS="$GL_LDFLAGS $X_LDFLAGS $saved_LDFLAGS"

for gl in GL MesaGL; do
  AC_MSG_CHECKING([${gl}])
  LIBS="$saved_LIBS ${X_LIBS} -l${gl}"
  AC_TRY_LINK( ,[glBegin(); ], have_GL=yes, have_GL=no)
  AC_MSG_RESULT($have_GL)
  if test "x${have_GL}" = xyes; then
    GL_LIBS="-l${gl}"
    break;
  fi
done
if test "x${have_GL}" = xno; then
  AC_MSG_CHECKING([Mesa with pthreads])
  LIBS="$saved_LIBS ${X_LIBS} -lMesaGL -lpthread"
  AC_TRY_LINK( ,[ char glBegin(); glBegin(); ], \
	      have_Mesa_pthread=yes, have_Mesa_pthread=no)
  AC_MSG_RESULT($have_Mesa_pthread)
  if test x$have_Mesa_pthread = xyes; then
    GL_LIBS="-lMesaGL -lpthread"
  else
    AC_MSG_ERROR(Couldn't find OpenGL library.)
  fi
fi

# test for GLU
for glu in GLU MesaGLU; do
  AC_MSG_CHECKING([${glu}])
  LIBS="$saved_LIBS ${X_LIBS} ${GL_LIBS} -l${glu}"
  AC_TRY_LINK( ,[ char gluLookAt(); gluLookAt(); ], have_GLU=yes, have_GLU=no)
  AC_MSG_RESULT($have_GLU)
  if test "x${have_GLU}" = xyes; then
    GL_LIBS="$GL_LIBS -l${glu}"
    break;
  fi
done
if test "x${have_GLU}" = xno; then
  AC_MSG_ERROR(Couldn't find OpenGL Utility library.)
fi

AC_CHECK_LIB(glut, glutSwapBuffers,,
  AC_MSG_ERROR(You need to have glut installed))
AC_CHECK_HEADER(GL/glut.h,,
  AC_MSG_ERROR(You need to have the glut headers installed))


AC_CHECK_HEADER(GL/gl.h,,
  AC_MSG_ERROR(You need to have the OpenGL headers installed))
AC_CHECK_HEADER(GL/glu.h,,
  AC_MSG_ERROR(You need to have the OpenGL headers installed))


AC_SUBST(GL_CFLAGS)
AC_SUBST(GL_LDFLAGS)
AC_SUBST(GL_LIBS)

CFLAGS=$saved_CFLAGS
LDFLAGS=$saved_LDFLAGS
LIBS=$saved_LIBS
########################################################


AC_SUBST(GL_CFLAGS)
AC_SUBST(GL_LIBS)


AC_OUTPUT([
Makefile
src/Makefile])
