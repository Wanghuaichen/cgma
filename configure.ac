###############################################################################
#                           Standard Stuff
################################################################################
AC_INIT(CGMA, 10.2.2)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(CGMA,10.2.2)
AC_DISABLE_SHARED

AC_PROG_SED

EXTRA_GNU_FLAGS="-Wall -pipe -pedantic"
SNL_CHECK_COMPILERS
AC_PROG_FC
AM_CONDITIONAL(build_parallel, [test "x$WITH_MPI" != "xno"])
AC_PROG_LIBTOOL
LIBS="-lm"
AC_PROG_LIBTOOL
AC_C_BIGENDIAN( [LITTLE_ENDIAN=], [LITTLE_ENDIAN=-DLITTLE_ENDIAN=BYTE_ORDER] )
AC_SUBST(LITTLE_ENDIAN)


################################################################################
#                Fortran Stuff for ITAPS C bindings
################################################################################
if test "x" != "x$FC"; then
  AC_FC_WRAPPERS
fi


################################################################################
#                              Extract libtool config
################################################################################
ITAPS_LIBTOOL_VAR( [CXX], [compiler_lib_search_path], [CGM_CXX_LINKFLAGS])
ITAPS_LIBTOOL_VAR( [CXX], [postdeps], [CGM_CXX_LIBS])
AC_SUBST(CGM_CXX_LINKFLAGS)
AC_SUBST(CGM_CXX_LIBS)


################################################################################
#                              MPI OPTIONS
################################################################################

if test "x$WITH_MPI" != "xno"; then
  CXXFLAGS="$CXXFLAGS -DUSE_MPI"
  CGM_PARALLEL_INCLUDE='-I${CGM_DIR}/geom/parallel'
  CGM_PARALLEL_LIB='-L${CGM_DIR}/geom/parallel -lcubit_parallel'
  CGM_PARALLEL_LIB_FILE='${CGM_LIBDIR}/libcubit_parallel.la'
fi
AM_CONDITIONAL(USE_MPI, [test "xno" != "x$WITH_MPI"])
AC_SUBST(CGM_PARALLEL_INCLUDE)
AC_SUBST(CGM_PARALLEL_LIB)
AC_SUBST(CGM_PARALLEL_LIB_FILE)

################################################################################
#                           CGM-specific Checks
################################################################################

SNL_CANT_USE_STD
SNL_TEMPLATE_DEFS_INCLUDED
AC_SUBST(CANT_USE_STD)
AC_SUBST(CANT_USE_STD_IO)
AC_SUBST(TEMPLATE_DEFS_INCLUDED)
AM_CONDITIONAL(INCLUDE_TEMPLATE_DEFS, test x$TEMPLATE_DEFS_INCLUDED != x)

################################################################################
#                         Use Cubit shared libraries
################################################################################
CUBIT_FILE=no
CUBIT_BIN_DIR=no
HAVE_ACIS_DEF=

AC_ARG_WITH( cubit,
             AC_HELP_STRING([--with-cubit=<dir>],[Use CGM from CUBIT shared library]),
             [CUBIT_DIR="$withval"
              DISTCHECK_CONFIGURE_FLAGS="$DISTCHECK_CONFIGURE_FLAGS --with-cubit=\"${withval}\""],
             [CUBIT_DIR=no])
AC_ARG_WITH( CUBIT,
             AC_HELP_STRING([--with-CUBIT=<dir>],[DEPRECATED.  Use --with-cubit (small case)]),
             [CUBIT_DIR="$withval"
              DISTCHECK_CONFIGURE_FLAGS="$DISTCHECK_CONFIGURE_FLAGS --with-cubit=\"${withval}\""])
if test "x$CUBIT_DIR" == "xyes"; then
  AC_MSG_ERROR([Option --with-CUBIT without specifying Cubit directory.])
elif test "x$CUBIT_DIR" != "xno"; then
  if ! test -d "$CUBIT_DIR"; then
    AC_MSG_ERROR([$CUBIT_DIR: not a directory.])
  fi
  CUBIT_LDFLAGS=
  CUBIT_LTFLAGS=
  CUBIT_LIBS=no
  if test -d "$CUBIT_DIR/bin"; then
    CUBIT_BIN_DIR="$CUBIT_DIR/bin"
  elif test -d "$CUBIT_DIR/lib"; then
    CUBIT_BIN_DIR="$CUBIT_DIR/lib"
  elif test -d "$CUBIT_DIR/libs"; then
    CUBIT_BIN_DIR="$CUBIT_DIR/libs"
  else
    CUBIT_BIN_DIR="$CUBIT_DIR"
  fi
  CUBIT_LDFLAGS="-L$CUBIT_BIN_DIR"
  CUBIT_LTFLAGS="-R$CUBIT_BIN_DIR"

  cat >conftest.cc <<EOCUBITCT
class AcisQueryEngine { public: static AcisQueryEngine* instance_; };
int main() {
  AcisQueryEngine::instance_ = 0;
  return 0;
}
EOCUBITCT
  pattern=["${CUBIT_BIN_DIR}/*cubiti[0-9][0-9].*"]
  for file in $pattern; do
    AC_MSG_CHECKING([for AcisQueryEngine::instance_ in $file])
    num=[`expr X"$file" : X".*cubiti\([0-9][0-9]\)\..*"`]
    if ./libtool --mode=link $CXX $LDFLAGS $CUBIT_LDFLAGS $CUBIT_LTFLAGS -lcubiti$num conftest.cc -o conftest >&5 2>&5; then
      CUBIT_LIBS="-lcubiti$num"
      CUBIT_FILE=`expr x"$file" : x"$CUBIT_BIN_DIR/\(.*\)"`
      CUBIT_FILE="${CUBIT_BIN_DIR}/$CUBIT_FILE"
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)
    fi
  done
  rm -f conftest.cc
  
  if test "x$CUBIT_LIBS" = "xno"; then
    AC_MSG_ERROR("Cubit library not found in $CUBIT_BIN_DIR")
  fi
  CGM_EXT_LIBS="$CGM_EXT_LIBS $CUBIT_LIBS"
  CGM_EXT_LDFLAGS="$CGM_EXT_LDFLAGS $CUBIT_LDFLAGS"
  CGM_EXT_LTFLAGS="$CGM_EXT_LTFLAGS $CUBIT_LTFLAGS"
  CUBIT_CGM="-DCUBIT_CGM"
dnl  AC_DEFINE( HAVE_ACIS, "Have CGM ACIS Implementation" )
  HAVE_ACIS_DEF=-DHAVE_ACIS
fi

AC_SUBST(CUBIT_BIN_DIR)
AC_SUBST(CUBIT_LDFLAGS)
AC_SUBST(CUBIT_LTFLAGS)
AC_SUBST(CUBIT_LIBS)
AC_SUBST(CUBIT_FILE)
AC_SUBST(CUBIT_CGM)

AM_CONDITIONAL(BUILD_CGM,[test x"$CUBIT_DIR" == x"no"])
AM_CONDITIONAL(WITH_CUBIT,[test x"$CUBIT_DIR" != x"no"])

################################################################################
#                           ACIS OPTIONS
################################################################################
ACIS_BASE_LIBS='-lSpaAVis -lSpaAWarp -lSpaASurf -lSpaALops -lSpaABlend -lSpaACIS -lSpaBase'
ACIS_STEP_LIBS='-lacisstep -lxstep'
ACIS_IGES_LIBS='-lacisiges -lxiges'

AC_ARG_WITH( acis, 
             AC_HELP_STRING([--with-acis=<dir>],[Build with ACIS support, specify directory where ACIS is installed.]),
             [ACIS_DIR=$withval
              DISTCHECK_CONFIGURE_FLAGS="$DISTCHECK_CONFIGURE_FLAGS --with-acis=\"${withval}\""],
              [ACIS_DIR=no] )
AC_ARG_WITH( ACIS, 
             AC_HELP_STRING([--with-ACIS],[DEPRECATED.  Use --wtih-acis (small case)]),
             [ACIS_DIR=$withval
              DISTCHECK_CONFIGURE_FLAGS="$DISTCHECK_CONFIGURE_FLAGS --with-acis=\"${withval}\""] )
AC_ARG_WITH( ACIS-system, AC_HELP_STRING([--with-acis-system=SYS], 
          [Specify ACIS system name (e.g. linux_so), default is to autodetect.]),
             [ACIS_SYSTEM=$withval],[ACIS_SYSTEM=] )
AC_ARG_WITH( acis-version, AC_HELP_STRING([--with-acis-version=INT],
[Specify ACIS version as an integer value (100*major+10*minor+point), default is to autodetect.]),
[if ! test "$withval" -gt "600"; then
  AC_MSG_ERROR("ACIS-version must be an integer greater than 600.")
 fi
 ACIS_VERSION=$withval],
[ACIS_VERSION=0])
if test "x$ACIS_DIR" == "x"; then 
  ACIS_DIR=no
fi          
if test "x$ACIS_DIR" == "xno"; then
  ACIS_LIB_DIR=
  ACIS_LINK_FLAGS=
fi
if test "x$ACIS_DIR" != "xno"; then
  if test "x$CUBIT_DIR" != "xno"; then
    AC_MSG_ERROR("Conflicting options:  Cannot specifiy both --with-ACIS and --with-CUBIT")
  fi

  AC_CHECK_FILE( [${srcdir}/geom/ACIS/AcisQueryEngine.hpp], [],
                 [AC_MSG_ERROR([ACIS source not present.  Checkout ACIS_SRC and move to ACIS])] )

  if test "$ACIS_DIR" == "yes"; then ACIS_DIR=.; fi
  SNL_ACIS_ENV
  
  SNL_ACIS_TRANSLATOR
  if test x$ACIS_STEP_TRANSLATOR != x; then
    ACIS_LIBS="$ACIS_LIBS $ACIS_STEP_LIBS"
    ACIS_XLIBS="$ACIS_XLATE_LIBS"
  fi
  if test x$ACIS_IGES_TRANSLATOR != x; then
    ACIS_LIBS="$ACIS_LIBS $ACIS_IGES_LIBS"
    ACIS_XLIBS="$ACIS_XLATE_LIBS"
  fi
  CGM_ACIS_LIBS='-lcubit_ACIS'
  ACIS_LIBS="$ACIS_LIBS $ACIS_XLIBS $ACIS_BASE_LIBS"
  ACIS_LINK_FLAGS="-L$ACIS_LIB_DIR"
  ACIS_HEALER="-DACIS_HEALER"
  ACIS_INCLUDES="-I$ACIS_DIR/include"
  ACIS_LTFLAGS="-R$ACIS_LIB_DIR"
  CGM_EXT_LIBS="$CGM_EXT_LIBS $ACIS_LIBS"
  CGM_EXT_LDFLAGS="$CGM_EXT_LDFLAGS $ACIS_LINK_FLAGS"
  CGM_EXT_INCLUDE="$CGM_EXT_INCLUDE $ACIS_INCLUDES"
  CGM_EXT_LTFLAGS="$CGM_EXT_LTFLAGS $ACIS_LTFLAGS"
  if test "x$GXX" = "xyes"; then
    CPPFLAGS="$CPPFLAGS -isystem $ACIS_DIR/include"
  fi
  ACIS_DEFS='$(ACIS_STEP_TRANSLATOR) $(ACIS_IGES_TRANSLATOR) -DCUBIT_ACIS_VERSION=$(ACIS_VERSION) -DACIS_VERSION=$(ACIS_VERSION) -D$(ACIS_PLATFORM) -DACIS_LOCAL_OPS'
dnl  AC_DEFINE( HAVE_ACIS )
  HAVE_ACIS_DEF=-DHAVE_ACIS
fi

# ACIS (at least up to version 16) requires new.h. 
# Newer C++ compilers may not provide this backwards-compatibility
# file.  Check if it is present, and if not, use our own.
ACIS_NEW_COMPAT_INC='-I${top_srcdir}/compat/new'
if test "x$ACIS_DIR" != "xno"; then
  AC_LANG_PUSH(C++)
  AC_CHECK_HEADER([new.h],[],[ACIS_INCLUDES="$ACIS_INCLUDES $ACIS_NEW_COMPAT_INC"])
  AC_LANG_POP(C++)
fi

AC_SUBST(ACIS_DEFS)
AC_SUBST(ACIS_DIR)
AC_SUBST(ACIS_LINK_FLAGS)
AC_SUBST(ACIS_LIB_DIR)
AC_SUBST(ACIS_INCLUDES)
AC_SUBST(ACIS_LIBS)
AC_SUBST(CGM_ACIS_LIBS)
AC_SUBST(ACIS_HEALER)
AC_SUBST(ACIS_VERSION)
AC_SUBST(ACIS_PLATFORM)
AC_SUBST(ACIS_STEP_TRANSLATOR)
AC_SUBST(ACIS_IGES_TRANSLATOR)
AC_SUBST(HAVE_ACIS_DEF)
AM_CONDITIONAL(build_ACIS, test x$ACIS_DIR != xno)

################################################################################
#                           iGeom Babel
################################################################################
AC_ARG_WITH(babel, 
[AC_HELP_STRING([--with-babel=DIR], [Specify directory containing babel])],
[BABEL_DIR=$withval
 DISTCHECK_CONFIGURE_FLAGS="$DISTCHECK_CONFIGURE_FLAGS --with-babel=\"${withval}\""
 ], [BABEL_DIR=no])
case "x$BABEL_DIR" in
  xno)
    BABEL_DIR=
    USE_BABEL=no
    ;;
  xyes|x)
    BABEL_DIR=/usr/local
    AC_CHECK_FILE([${BABEL_DIR}/bin/babel], [USE_BABEL=yes], [USE_BABEL=no])
    ;;
  *)
    AC_CHECK_FILE([${BABEL_DIR}/bin/babel], [USE_BABEL=yes], [USE_BABEL=no])
    ;;
esac
AC_SUBST(BABEL_DIR)
AC_SUBST(USE_BABEL)
AM_CONDITIONAL(USE_BABEL, [test "xno" != "x$USE_BABEL"])


################################################################################
#                           OCC Options 
################################################################################
# Add --with-occ option to configure script

AC_ARG_WITH( occ,
             [AC_HELP_STRING([--with-occ=<dir>],[OpenCascade shared library directory])],
             [occ_DIR="$withval"
              DISTCHECK_CONFIGURE_FLAGS="$DISTCHECK_CONFIGURE_FLAGS --with-occ=\"${withval}\""],
             [occ_DIR=no] )

# if user specified option (other than --without-occ)
CUBIT_OCC_LIB=
HAVE_OCC_DEF=
if test "x$occ_DIR" != "xno"; then
  OCC_LIBS="-lTKMath -lTKXSBase -lTKernel -lTKShHealing -lTKBRep -lTKG3d -lTKBO -lTKFeat -lTKFillet -lTKG2d -lTKOffset -lTKBool -lTKGeomAlgo -lTKGeomBase -lTKPrim -lTKTopAlgo -lTKHLR -lTKIGES -lTKMesh -lTKPrim -lTKSTEP209 -lTKSTEPAttr -lTKSTEPBase -lTKSTEP -lTKSTL -lTKTopAlgo -lTKXSBase -lTKLCAF"
  CUBIT_OCC_LIB="-lcubit_OCC"

  # Set OCC_INC_FLAG and OCC_LIB_FLAG based on --with-occ option
  if test "x$occ_DIR" = "x"; then
    OCC_INC_FLAG=
    OCC_LIB_DIR=
    OCC_LIB_FLAG=
  else
    AC_CACHE_CHECK( [for 'include' subdir of $occ_DIR], 
                    [ac_cv_occ_include_dir],
                    [ac_cv_occ_include_dir=NONE
                     for subdir in inc ros/inc include; do
                       if test -f "${occ_DIR}/${subdir}/Standard_Version.hxx"; then
                         ac_cv_occ_include_dir="$subdir"
                         break
                       elif test -d "${occ_DIR}/${subdir}"; then
                         ac_cv_occ_include_dir="$subdir";
                       fi
                     done] )
    if test "xNONE" = "x$ac_cv_occ_include_dir"; then
      AC_MSG_ERROR([Count not find include dir in: $occ_DIR])
    else
      OCC_INC_FLAG="-I${occ_DIR}/$ac_cv_occ_include_dir"
    fi
    
    AC_CACHE_CHECK( [for 'lib' subdir of $occ_DIR],
                    [ac_cv_occ_lib_dir],
                    [ac_cv_occ_lib_dir=NONE
                     uname=`uname`
                     for subdir in lib ros/lib ros/${uname}/lib ros/$uname ; do
                       if test -d "${occ_DIR}/${subdir}"; then
                         ac_cv_occ_lib_dir="$subdir"
                         for file in ${occ_DIR}/${subdir}/libTKernel.* ; do
                           if test -f $file; then
                             break 2
                           fi
                         done
                       fi
                     done] )
    if test "xNONE" = "x$ac_cv_occ_lib_dir"; then
      AC_MSG_ERROR([Count not find lib dir in: $occ_DIR])
    else
      OCC_LIB_DIR="$occ_DIR/$ac_cv_occ_lib_dir"
      OCC_LIB_FLAG="-L$OCC_LIB_DIR"
    fi
  fi

  # Check of OCC is present and working

  # Save old value of these variables and update working ones
  old_CPPFLAGS="$CPPFLAGS"
  old_LDFLAGS="$LDFLAGS"
  CPPFLAGS="$CPPFLAGS ${OCC_INC_FLAG}"
  LDFLAGS="$LDFLAGS -L$OCC_LIB_DIR"

  # Check if 64-bit
  AC_MSG_CHECKING([if platform is 64-bit (-D_OCC64)])
  AC_TRY_COMPILE([],[int array[6-(int)sizeof(void*)];],[AC_MSG_RESULT(no)],
                 [AC_MSG_RESULT(yes); OCC_INC_FLAG="$OCC_INC_FLAG -D_OCC64"])

  # OCC requires some defines
  AC_LANG_PUSH(C++)
  AC_CHECK_HEADER([iostream],[OCC_INC_FLAG="$OCC_INC_FLAG -DHAVE_IOSTREAM"])
  AC_CHECK_HEADER([iostream.h],[OCC_INC_FLAG="$OCC_INC_FLAG -DHAVE_IOSTREAM_H"])
  AC_CHECK_HEADER([iomanip],[OCC_INC_FLAG="$OCC_INC_FLAG -DHAVE_IOMANIP"])
  AC_CHECK_HEADER([iomanip.h],[OCC_INC_FLAG="$OCC_INC_FLAG -DHAVE_IOMANIP_H"])
  AC_CHECK_HEADER([fstream],[OCC_INC_FLAG="$OCC_INC_FLAG -DHAVE_FSTREAM"])
  AC_CHECK_HEADER([fstream.h],[OCC_INC_FLAG="$OCC_INC_FLAG -DHAVE_FSTREAM_H"])
  AC_CHECK_HEADER([limits.h],[OCC_INC_FLAG="$OCC_INC_FLAG -DHAVE_LIMITS_H"])

  # Check for OCC headers Stadard_Version.hxx
  AC_CHECK_HEADER( [Standard_Version.hxx], [],
                 [AC_MSG_ERROR([OpenCascade config error:Standard_Version.hxx not found])] )

  # Check if libTKernel.so contains function 'create_mailbox'
  # OCC 6.3 has build error where libTKernel.so requires libdl.so,
  #   but does not list that in the shared library dependencies.
  #   Try to work around this bug by detecting the need for -ldl.
  AC_CHECK_LIB( [TKernel], [create_mailbox],,
                [unset ac_cv_lib_TKernel_create_mailbox
                 AC_CHECK_LIB( [TKernel], [create_mailbox],
                               [OCC_LIBS="$OCC_LIBS -ldl"],
                               [AC_MSG_ERROR([OpenCascade config error: libTKernel not found ])],
                               [-ldl] )
                ] )
  AC_LANG_POP(C++)

  # Restore original values of variables
  CPPFLAGS="$old_CPPFLAGS"
  LDFLAGS="$old_LDFLAGS"
  HAVE_OCC_DEF="-DHAVE_OCC"
  
  # Append to CGM global values
  CGM_EXT_LIBS="$CGM_EXT_LIBS $OCC_LIBS"
  CGM_EXT_LDFLAGS="$CGM_EXT_LDFLAGS $OCC_LIB_FLAG"
  CGM_EXT_INCLUDE="$CGM_EXT_INCLUDE $OCC_INC_FLAG"
  # NOTE: No CGM_EXT_LTFLAGS because OCC libs are .la files (not needed)
fi

# Export these variables as variables in Makefiles
AC_SUBST(OCC_INC_FLAG)
AC_SUBST(OCC_LIB_DIR)
AC_SUBST(OCC_LIB_FLAG)
AC_SUBST(OCC_LIBS)
AC_SUBST(CUBIT_OCC_LIB)
AC_SUBST(HAVE_OCC_DEF)

################################################################################
#                           Define variables for linking
################################################################################
AC_SUBST(CGM_EXT_LIBS)
AC_SUBST(CGM_EXT_LDFLAGS)
AC_SUBST(CGM_EXT_LTFLAGS)
AC_SUBST(CGM_EXT_INCLUDE)

# Allow "if build_OCC" in Makefile.am
AM_CONDITIONAL( build_OCC, [test "x$occ_DIR" != "xno"] )


################################################################################
#                           Output Files
################################################################################
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(DISTCHECK_CONFIGURE_FLAGS)
AC_MSG_RESULT([CXXFLAGS = $CXXFLAGS])
dnl AC_OUTPUT_COMMANDS( [if test -f util/CubitUtilConfigure.h; then true; else echo "#define CUBIT_UTIL_EXPORT" > util/CubitUtilConfigure.h; fi],
dnl                     [if test -f geom/CubitGeomConfigure.h; then true; else echo "#define CUBIT_GEOM_EXPORT" > geom/CubitGeomConfigure.h; fi] )
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_HEADERS(itaps/iBase_FCDefs.h)
AC_CONFIG_FILES(Makefile 
           util/Makefile
           geom/Makefile
           geom/ACIS/Makefile
           geom/Cholla/Makefile
           geom/facet/Makefile
           geom/facetbool/Makefile
           geom/OCC/Makefile
           geom/parallel/Makefile
           geom/virtual/Makefile 
           init/Makefile
           compat/Makefile
           cgm.make
           itaps/Makefile
           itaps/iGeom-Defs.inc
	   itaps/SIDL/Makefile
	   itaps/SIDL/common.make
           itaps/SIDL/iGeom-SIDL-Defs.inc
           test/Makefile
           cgm_apps/Makefile
           cgm_apps/examples/Makefile
           cgm_apps/examples/driverc++/Makefile
           cgm_apps/examples/facetdriver/Makefile
	   )

AC_OUTPUT
if test "x$CUBIT_DIR" != "xno" && test "x$CUBIT_DIR" != "x"; then
   AC_MSG_WARN([Linking against Cubit-based CGM; you'll need to add Cubit's 'bin' directory
                to your LD_LIBRARY_PATH before linking, or add -R (linux) or equivalent (non-linux)
                to your link statement to link applications.])
fi