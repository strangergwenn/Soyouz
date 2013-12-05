
#include "AmbientLight.h"
#include "GeomUtils.h"

#include "Engine/Rendering/renderer.hpp"

using namespace Ogre;

AmbientLight::AmbientLight()
{
	setRenderQueueGroup(RENDER_QUEUE_2);

	mRenderOp.vertexData = new VertexData();
	mRenderOp.indexData = 0;

	GeomUtils::createQuad(mRenderOp.vertexData);

	mRenderOp.operationType = RenderOperation::OT_TRIANGLE_STRIP; 
	mRenderOp.useIndexes = false; 

	// Set bounding
	setBoundingBox(AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));
	mRadius = 15000;

	mMatPtr = MaterialManager::getSingleton().getByName("DeferredShading/AmbientLight");
	assert(mMatPtr.isNull()==false);
	mMatPtr->load();

    // Explicitly bind samplers for OpenGL
    if(Root::getSingleton().getRenderSystem()->getName().find("OpenGL 3+") != String::npos)
    {
        mMatPtr->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("Tex0", 0);
        mMatPtr->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("Tex1", 1);
        mMatPtr->getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant("Tex2", 2);
    }
}

AmbientLight::~AmbientLight()
{
	// need to release IndexData and vertexData created for renderable
	delete mRenderOp.indexData;
	delete mRenderOp.vertexData;
}

Real AmbientLight::getBoundingRadius(void) const
{
	return mRadius;
}

Real AmbientLight::getSquaredViewDepth(const Camera*) const
{
	return 0.0;
}

const MaterialPtr& AmbientLight::getMaterial(void) const
{
	return mMatPtr;
}

void AmbientLight::getWorldTransforms(Ogre::Matrix4* xform) const
{
	*xform = Matrix4::IDENTITY;
}
void AmbientLight::updateFromCamera(Ogre::Camera* camera)
{
	Ogre::Technique* tech = getMaterial()->getBestTechnique();
	Ogre::Vector3 farCorner = camera->getViewMatrix(true) * camera->getWorldSpaceCorners()[4];

	for (unsigned short i=0; i<tech->getNumPasses(); i++) 
	{
		Ogre::Pass* pass = tech->getPass(i);
		Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);
	    
		params = pass->getFragmentProgramParameters();
		if (params->_findNamedConstantDefinition("farCorner"))
			params->setNamedConstant("farCorner", farCorner);
	}
}
