/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef _SSAO_OPERATION_H
#define _SSAO_OPERATION_H


#include "Engine/Rendering/renderer.hpp"
#include "OgreCompositorInstance.h"


/*----------------------------------------------
	SSAO operation
----------------------------------------------*/

class SSAOListener : public Ogre::CompositorInstance::Listener
{

public:

	void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);

	void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
  
};


#endif
