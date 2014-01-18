/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/Rendering/ssaolistener.hpp"


/*----------------------------------------------
	SSAO operation
----------------------------------------------*/


void SSAOListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
	// Data
	Ogre::Camera* cam = Ogre::Root::getSingletonPtr()->getSceneManager("GameScene")->getCurrentViewport()->getCamera();
	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
}

void SSAOListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
	// Data
	Ogre::Camera* cam = Ogre::Root::getSingletonPtr()->getSceneManager("GameScene")->getCurrentViewport()->getCamera();
	Ogre::SceneNode* cameraSpace = cam->getParentSceneNode();
	Ogre::Pass* pass = mat->getTechnique(0)->getPass(0);
		
	// Far corner update
	Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
	if (params->_findNamedConstantDefinition("farCorner"))
	{
		Ogre::Vector3 corner = cam->getViewMatrix() * cam->getWorldSpaceCorners()[4];
		params->setNamedConstant("farCorner", corner);
			Ogre::LogManager::getSingletonPtr()->logMessage((
				"corner = " +
				Ogre::StringConverter::toString(corner)
				).c_str());
	}

	// Transformation update
	params = pass->getFragmentProgramParameters();
	if (params->_findNamedConstantDefinition("cProj"))
	{
		Ogre::Matrix4 viewMatrix = cam->getViewMatrix();
		//viewMatrix.setTrans(Ogre::Vector3(0, 0, 0));
		
		params->setNamedConstant("cProj", cam->getViewMatrix());
		params->setNamedConstant("cCamPosition", cameraSpace->getPosition());

			Ogre::LogManager::getSingletonPtr()->logMessage((
				"mat = " +
				Ogre::StringConverter::toString(viewMatrix) +
				"pos = " +
				Ogre::StringConverter::toString(cameraSpace->getPosition())
				).c_str());


	}
}
  
