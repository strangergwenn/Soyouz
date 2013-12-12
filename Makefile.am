AUTOMAKE_OPTIONS = subdir-objects

MAINTAINERCLEANFILES = \
	configure \
	aclocal.m4 \
	Makefile.in

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = autogen.sh LICENSE README.md Soyouz.sln  Soyouz.vcxproj  Soyouz.vcxproj.user

bin_PROGRAMS=Soyouz

SoyouzCPPFiles= \
	Sources/main.cpp \
	Sources/Game/ship.cpp \
	Sources/Game/thruster.cpp \
	Sources/Game/weapon.cpp \
	Sources/Game/machinegun.cpp \
	Sources/Game/bullet.cpp \
	Sources/Engine/actor.cpp \
	Sources/Engine/meshactor.cpp \
	Sources/Engine/game.cpp \
	Sources/Engine/lightactor.cpp \
	Sources/Engine/iomanager.cpp \
	Sources/Engine/player.cpp \
	Sources/Engine/savable.cpp \
	Sources/Engine/Rendering/renderer.cpp \
	Sources/Engine/Rendering/renderoperation.cpp \
	Sources/Engine/Rendering/ambient.cpp \
	Sources/Engine/Rendering/deferredlight.cpp \
	Sources/Engine/Rendering/geometry.cpp \
	Sources/Engine/Rendering/lightmaterial.cpp \
	External/tinyxml2/tinyxml2.cpp
	
SoyouzHPPFiles= \
	Sources/Engine/game.hpp \
	Sources/Engine/player.hpp \
	Sources/Engine/iomanager.hpp \
	Sources/Engine/bullet.hpp \
	Sources/Engine/meshactor.hpp \
	Sources/Engine/lightactor.hpp \
	Sources/Engine/actor.hpp \
	Sources/Engine/savable.hpp \
	Sources/Engine/Rendering/renderer.hpp \
	Sources/Engine/Rendering/renderoperation.hpp \
	Sources/Engine/Rendering/ambient.hpp \
	Sources/Engine/Rendering/deferredlight.hpp \
	Sources/Engine/Rendering/geometry.hpp \
	Sources/Engine/Rendering/lightmaterial.hpp \
	Sources/Game/demoPlayer.hpp \
	Sources/Game/demoWorld.hpp \
	Sources/Game/ship.hpp \
	Sources/Game/thruster.hpp \
	Sources/Game/weapon.hpp \
	Sources/Game/machinegun.hpp \
	External/tinyxml2/tinyxml2.hpp
	


Soyouz_SOURCES= ${SoyouzCPPFiles} ${SoyouzHPPFiles}

Soyouz_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) ${BULLET_CFLAGS} -I$(top_srcdir)/External/tinyxml2
Soyouz_LDADD= $(OGRE_LIBS) $(OIS_LIBS) ${BULLET_LIBS}

install-data-local:
	@if [ -n "$${TRUEINSTALL}" ] ; then \
		$(mkinstalldirs) $(shell find @abs_top_srcdir@/Content @abs_top_srcdir@/Config f-type d -print) ; \
		for datasrc in $(shell find @abs_top_srcdir@/Content @abs_top_srcdir@/Config f-type f -print) ; do \
		datadest=`dirname $$(echo $${datasrc} |sed "s|@abs_top_srcdir@|$${DESTDIR}/$(prefix)|")` ; \
		$(install_sh) -c $${datasrc} $${datadest} ; \
		done ; \
	else \
		[ ! -e $${DESTDIR}/$(prefix)/Content ] && @$(LN_S) $(abs_top_srcdir)/Content $${DESTDIR}/$(prefix)/Content ; \
		[ ! -e $${DESTDIR}/$(prefix)/Config ] && @$(LN_S) $(abs_top_srcdir)/Config $${DESTDIR}/$(prefix)/Config ; \
	fi

