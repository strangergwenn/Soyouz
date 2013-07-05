/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ENGINE_H_
#define __ENGINE_H_


#include "Ogre.h"
#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreSceneManager.h"
#include "OgreOverlaySystem.h"
#include "OgreStringConverter.h"


#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
#    if OGRE_USE_D3D11
#      define OGRE_STATIC_Direct3D11
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  include "OgreStaticPluginLoader.h"
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#   include "macUtils.h"
#endif


#ifdef USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif


#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>


using namespace Ogre;


#endif /* __ENGINE_H_ */