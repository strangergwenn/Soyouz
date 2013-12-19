/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/Rendering/ambient.hpp"
#include "Engine/Rendering/geometry.hpp"


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

AmbientLight::AmbientLight()
	: SimpleRenderable()
{
	setRenderQueueGroup(Ogre::RENDER_QUEUE_2);

	// Render op setup
	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.indexData = 0;
	GeomUtils::createQuad(mRenderOp.vertexData);
	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP; 
	mRenderOp.useIndexes = false; 

	// Set bounding box (pretty much infinite)
	setBoundingBox(Ogre::AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));
	mRadius = 15000;

	// Ambient lighting material
	mMatPtr = Ogre::MaterialManager::getSingleton().getByName("Render/AmbientLight");
	assert(mMatPtr.isNull() == false);
	mMatPtr->load();
	
	// Explicitly bind samplers for OpenGL
	if (Ogre::Root::getSingleton().getRenderSystem()->getName().find("OpenGL 3+") != Ogre::String::npos)
	{
		mMatPtr->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("Tex0", 0);
		mMatPtr->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("Tex1", 1);
		mMatPtr->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("Tex2", 2);
	}
}


AmbientLight::~AmbientLight()
{
	delete mRenderOp.indexData;
	delete mRenderOp.vertexData;
}


/*----------------------------------------------
	Methods
----------------------------------------------*/

Ogre::Real AmbientLight::getBoundingRadius(void) const
{
	return mRadius;
}


Ogre::Real AmbientLight::getSquaredViewDepth(const Ogre::Camera*) const
{
	return 0.0;
}


const Ogre::MaterialPtr& AmbientLight::getMaterial(void) const
{
	return mMatPtr;
}


void AmbientLight::getWorldTransforms(Ogre::Matrix4* xform) const
{
	*xform = Ogre::Matrix4::IDENTITY;
}


void AmbientLight::updateFromCamera(Ogre::Camera* camera)
{
	Ogre::Technique* tech = getMaterial()->getBestTechnique();
	Ogre::Vector3 farCorner = camera->getViewMatrix(true) * camera->getWorldSpaceCorners()[4];

	for (unsigned short i = 0; i < tech->getNumPasses(); i++) 
	{
		// Vertex params
		Ogre::Pass* pass = tech->getPass(i);
		Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
		{
			params->setNamedConstant("farCorner", farCorner);
		}
	    
		// Fragment params
		params = pass->getFragmentProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
		{
			params->setNamedConstant("farCorner", farCorner);
		}
	}
}
