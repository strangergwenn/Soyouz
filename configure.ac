AC_PREREQ(2.59)

AC_INIT([Soyouz], [0.1.0])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE([enable])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_LN_S

#
# libtool library versioning stuff
#
# Library code modified:            REVISION++
# Interfaces changed/added/removed: CURRENT++   REVISION=0
# Interfaces added:                 AGE++
# Interfaces removed:               AGE=0

m4_define([LT_CURRENT],[0])
m4_define([LT_REVISION],[1])
m4_define([LT_AGE],[0])

LT_VERSION_INFO="LT_CURRENT:LT_REVISION:LT_AGE"
AC_SUBST([LT_VERSION_INFO])

includeflags=" \
-I\${top_srcdir}/Sources/ \
-I\${top_srcdir}/External/Ogre/include/OGRE/"

#
# Checks for programs.
#
AC_PROG_CC
AC_PROG_CXX
LT_INIT


# automake initialisation (mandatory) and check for minimal automake API version 1.11
AM_INIT_AUTOMAKE([1.11] no-define foreign)
AM_SILENT_RULES([no])


AC_MSG_CHECKING([whether to enable Werror])
AC_ARG_ENABLE(werror,
    AS_HELP_STRING([--enable-werror], [enable production mode (Werror) @<:@default=no@:>@]),
	[case "$enableval" in
	y | yes) CONFIG_WERROR=yes ;;
        *) CONFIG_WERROR=no ;;
    esac],
    [CONFIG_WERROR=no])
AC_MSG_RESULT([${CONFIG_WERROR}])
if test "${CONFIG_WERROR}" = "yes"; then
    CFLAGSTMP="${CFLAGSTMP} -Werror"
fi

#
# True install
#
AC_MSG_CHECKING([whether to enable trueinstall])
AC_ARG_ENABLE(trueinstall,
    AS_HELP_STRING([--enable-trueinstall], [enable true install (copy Content & Config dirs  @<:@default=no@:>@]),
	[case "$enableval" in
	y | yes) CONFIG_TRUEINSTALL=yes ;;
        *) CONFIG_WERROR=no ;;
    esac],
    [CONFIG_TRUEINSTALL=no])
AC_MSG_RESULT([${CONFIG_TRUEINSTALL}])
if test "${CONFIG_TRUEINSTALL}" = "yes"; then
    TRUEINSTALL=yes
    AC_SUBST(TRUEINSTALL)
fi

#
# Debugging
#
AC_MSG_CHECKING([whether to enable debugging])
AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug], [enable debugging @<:@default=yes@:>@]),
	[case "$enableval" in
	y | yes) CONFIG_DEBUG=yes ;;
        *) CONFIG_DEBUG=no ;;
    esac],
    [CONFIG_DEBUG=yes])
AC_MSG_RESULT([${CONFIG_DEBUG}])
if test "${CONFIG_DEBUG}" = "yes"; then
    CFLAGSTMP="${CFLAGSTMP} -g"
    AC_DEFINE(DEBUG, 1, [debugging])
else
    CFLAGSTMP="${CFLAGSTMP} -O3"
fi
AC_PREFIX_DEFAULT (.)
bindir=${prefix}
libdir=${prefix}

#
# Ptxdist platform directory
#
AC_MSG_CHECKING([OGRE DIR])
AC_ARG_WITH(ogre,
            [AS_HELP_STRING([--with-ogre=DIR], [Path to the OGRE directory])],
            [ogre_dir=${withval}],
            [ogre_dir=no])
AC_MSG_RESULT([${ogre_dir}])
if test "${ogre_dir}" = "no"; then
#	OGRE_PATH="${abs_top_srcdir}/./External/Ogre"
	OGRE_PATH="External/Ogre"
	AC_SUBST(OGRE_PATH)
else
	OGRE_PATH=${ogre_dir}
fi


#PKG_CHECK_MODULES(OGRE, [OGRE >= 1.9 OGRE-Overlay >= 1.9 OGRE-PCZ >= 1.9 OGRE-Paging >= 1.9  OGRE-Property >= 1.9  OGRE-RTShaderSystem >= 1.9  OGRE-Terrain >= 1.9  OGRE-Volume >= 1.9])
#AC_SUBST(OGRE_CFLAGS)
#AC_SUBST(OGRE_LIBS)

PKG_CHECK_MODULES(OIS, [OIS >= 1.0])
AC_SUBST(OIS_CFLAGS)
AC_SUBST(OIS_LIBS)

PKG_CHECK_MODULES(BULLET,[bullet >= 2.81])
AC_SUBST(BULLET_CFLAGS)
AC_SUBST(BULLET_LIBS)

CFLAGS="${CFLAGS} -W -Wall ${CFLAGSTMP}"
CXXFLAGS="${CXXFLAGS} -W -Wall ${CFLAGSTMP}"
LDFLAGS="${LDFLAGS} -no-undefined"
CPPFLAGS="${CPPFLAGS} ${includeflags}"

OGRE_LIBS="-L${OGRE_PATH}/lib -lOgreMain -lpthread  -lboost_system -lOgreTerrain -lOgrePaging -lOgreRTShaderSystem -lOgreProperty ${OGRE_PATH}/lib/OGRE/Plugin_PCZSceneManager.so ${OGRE_PATH}/lib/OGRE/Plugin_OctreeZone.so ${OGRE_PATH}/lib/OGRE/RenderSystem_GL.so -lOgreOverlay -Wl,-rpath ./${OGRE_PATH}/lib/OGRE/ -Wl,-rpath ./${OGRE_PATH}/lib/"
OGRE_CFLAGS="-pthread -I${OGRE_PATH}/include/OGRE/Property -I${OGRE_PATH}/include/OGRE/Plugins/PCZSceneManager -I${OGRE_PATH}/include/OGRE/Plugins/OctreeZone -I${OGRE_PATH}/include/OGRE/Terrain -I${OGRE_PATH}/include/OGRE/Paging -I${OGRE_PATH}/include/OGRE/RTShaderSystem -I${OGRE_PATH}/include/OGRE -I${OGRE_PATH}/include/OGRE/Overlay/"
AC_SUBST(OGRE_LIBS)
AC_SUBST(OGRE_CFLAGS)

AC_CONFIG_FILES([
Makefile 
])
AC_OUTPUT

