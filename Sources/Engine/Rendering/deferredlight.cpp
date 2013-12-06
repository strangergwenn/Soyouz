/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/Rendering/deferredlight.hpp"
#include "Engine/Rendering/geometry.hpp"


#define ENABLE_BIT(mask, flag) (mask) |= (flag)
#define DISABLE_BIT(mask, flag) (mask) &= ~(flag)


/*----------------------------------------------
	Constructor & destructor
----------------------------------------------*/

DeferredLight::DeferredLight(LightMaterialGenerator *sys, Ogre::Light* parentLight):
    mParentLight(parentLight), bIgnoreWorld(false), mGenerator(sys), mPermutation(0)
{
	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = 0;
	mRenderOp.vertexData = 0;
	mRenderOp.useIndexes = true;

	updateFromParent();
}


DeferredLight::~DeferredLight()
{
    delete mRenderOp.indexData;
    delete mRenderOp.vertexData;
}


/*----------------------------------------------
	Methods
----------------------------------------------*/


void DeferredLight::setAttenuation(float c, float b, float a)
{
	// Set Attenuation parameter to shader
	// setCustomParameter(3, Vector4(c, b, a, 0));
	float outerRadius = mParentLight->getAttenuationRange();

	if (c != 1.0f || b != 0.0f || a != 0.0f)
	{
		ENABLE_BIT(mPermutation, LightMaterialGenerator::MI_ATTENUATED);
		if (mParentLight->getType() == Ogre::Light::LT_POINT)
		{
			//// Calculate radius from Attenuation
			int threshold_level = 10;// difference of 10-15 levels deemed unnoticeable
			float threshold = 1.0f/((float)threshold_level/256.0f); 

			//// Use quadratic formula to determine outer radius
			c = c-threshold;
			float d=sqrt(b*b-4*a*c);
			outerRadius = (-2*c)/(b+d);
			outerRadius /= 1.2f;
		}
	}
	else
	{
		DISABLE_BIT(mPermutation,LightMaterialGenerator::MI_ATTENUATED);
	}
    
	rebuildGeometry(outerRadius);
}


void DeferredLight::setSpecularColour(const Ogre::ColourValue &col)
{	
	if(col.r != 0.0f || col.g != 0.0f || col.b != 0.0f)
	{
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_SPECULAR);
	}
	else
	{
		DISABLE_BIT(mPermutation,LightMaterialGenerator::MI_SPECULAR);
	}	
}


void DeferredLight::rebuildGeometry(float radius)
{
	//Disable all 3 bits
	DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_POINT);
	DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_SPOTLIGHT);
	DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_DIRECTIONAL);

	switch (mParentLight->getType())
	{
	case Ogre::Light::LT_DIRECTIONAL:
		createRectangle2D();
        ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_DIRECTIONAL);
		break;
	case Ogre::Light::LT_POINT:
		/// XXX some more intelligent expression for rings and segments
		createSphere(radius, 10, 10);
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_POINT);
		break;
	case Ogre::Light::LT_SPOTLIGHT:
		Ogre::Real height = mParentLight->getAttenuationRange();
		Ogre::Radian coneRadiusAngle = mParentLight->getSpotlightOuterAngle() / 2;
        Ogre::Real rad = Ogre::Math::Tan(coneRadiusAngle) * height;
		createCone(rad, height, 20);
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_SPOTLIGHT);
		break;
	}	
}


void DeferredLight::createRectangle2D()
{
	delete mRenderOp.vertexData; 
	delete mRenderOp.indexData; 

	// Create rectangle
	mRenderOp.vertexData = new Ogre::VertexData();
    mRenderOp.indexData = 0;
	GeomUtils::createQuad(mRenderOp.vertexData);
    mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_STRIP; 
    mRenderOp.useIndexes = false; 

	// Set bounding box
    setBoundingBox(Ogre::AxisAlignedBox(-10000,-10000,-10000,10000,10000,10000));
	mRadius = 15000;
	bIgnoreWorld = true;
}


void DeferredLight::createSphere(float radius, int nRings, int nSegments)
{
	delete mRenderOp.vertexData; 
	delete mRenderOp.indexData;
	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = new Ogre::IndexData();
	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.useIndexes = true;

	GeomUtils::createSphere(mRenderOp.vertexData, mRenderOp.indexData
		, radius
		, nRings, nSegments
		, false // no normals
		, false // no texture coordinates
		);

	// Set bounding box and sphere
	setBoundingBox(Ogre::AxisAlignedBox(Ogre::Vector3(-radius, -radius, -radius), Ogre::Vector3(radius, radius, radius) ) );
	mRadius = radius;
	bIgnoreWorld = false;
}


void DeferredLight::createCone(float radius, float height, int nVerticesInBase)
{
	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;
	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.indexData = new Ogre::IndexData();
	mRenderOp.vertexData = new Ogre::VertexData();
	mRenderOp.useIndexes = true;

	GeomUtils::createCone(mRenderOp.vertexData, mRenderOp.indexData
		, radius
		, height, nVerticesInBase);

	// Set bounding box and sphere
	setBoundingBox(Ogre::AxisAlignedBox( 
			Ogre::Vector3(-radius, 0, -radius), 
			Ogre::Vector3(radius, height, radius) ) );

	mRadius = radius;
	bIgnoreWorld = false;
}


Ogre::Real DeferredLight::getBoundingRadius(void) const
{
	return mRadius;
}


Ogre::Real DeferredLight::getSquaredViewDepth(const Ogre::Camera* cam) const
{
	if(bIgnoreWorld)
	{
		return 0.0f;
	}
	else
	{
		Ogre::Vector3 dist = cam->getDerivedPosition() - getParentSceneNode()->_getDerivedPosition();
		return dist.squaredLength();
	}
}


const Ogre::MaterialPtr& DeferredLight::getMaterial(void) const
{
	return mGenerator->getMaterial(mPermutation);
}


void DeferredLight::getWorldTransforms(Ogre::Matrix4* xform) const
{
	if (mParentLight->getType() == Ogre::Light::LT_SPOTLIGHT)
	{
		Ogre::Quaternion quat = Ogre::Vector3::UNIT_Y.getRotationTo(mParentLight->getDerivedDirection());
		xform->makeTransform(mParentLight->getDerivedPosition(),
			Ogre::Vector3::UNIT_SCALE, quat);
	}
	else
	{
		xform->makeTransform(mParentLight->getDerivedPosition(),
			Ogre::Vector3::UNIT_SCALE, Ogre::Quaternion::IDENTITY);
	}
	
}


void DeferredLight::updateFromParent()
{
	setAttenuation(mParentLight->getAttenuationConstant(), mParentLight->getAttenuationLinear(), mParentLight->getAttenuationQuadric());	
	setSpecularColour(mParentLight->getSpecularColour());

	if (getCastShadows())
	{
		ENABLE_BIT(mPermutation,LightMaterialGenerator::MI_SHADOW_CASTER);
	}
	else
	{
		DISABLE_BIT(mPermutation, LightMaterialGenerator::MI_SHADOW_CASTER);
	}
}


bool DeferredLight::isCameraInsideLight(Ogre::Camera* camera)
{
	switch (mParentLight->getType())
	{
	case Ogre::Light::LT_DIRECTIONAL:
		return false;

	case Ogre::Light::LT_POINT:
		{
			Ogre::Real distanceFromLight = camera->getDerivedPosition().distance(mParentLight->getDerivedPosition());
			return distanceFromLight <= mRadius + camera->getNearClipDistance() + 0.1;
		}

	case Ogre::Light::LT_SPOTLIGHT:
		{
			Ogre::Vector3 lightPos = mParentLight->getDerivedPosition();
			Ogre::Vector3 lightDir = mParentLight->getDerivedDirection();
			Ogre::Radian attAngle = mParentLight->getSpotlightOuterAngle();
		
			//Extend the analytic cone's radius by the near clip range by moving its tip accordingly.
			//Some trigonometry needed here.
			Ogre::Vector3 clipRangeFix = -lightDir * (camera->getNearClipDistance() / Ogre::Math::Tan(attAngle/2));
			lightPos = lightPos + clipRangeFix;
    
			Ogre::Vector3 lightToCamDir = camera->getDerivedPosition() - lightPos;
			Ogre::Real distanceFromLight = lightToCamDir.normalise();

			Ogre::Real cosAngle = lightToCamDir.dotProduct(lightDir);
			Ogre::Radian angle = Ogre::Math::ACos(cosAngle);
			//Check whether we will see the cone from our current POV.
			return (distanceFromLight <= (mParentLight->getAttenuationRange() + clipRangeFix.length()))
				&& (angle <= attAngle);
		}

	default:
		//Please the compiler
		return false;
	}
}


bool DeferredLight::getCastShadows() const
{
	return 
		mParentLight->_getManager()->isShadowTechniqueInUse() &&
		mParentLight->getCastShadows() && 
		(mParentLight->getType() == Ogre::Light::LT_DIRECTIONAL || mParentLight->getType() == Ogre::Light::LT_SPOTLIGHT);
}


void DeferredLight::updateFromCamera(Ogre::Camera* camera)
{
	const Ogre::MaterialPtr& mat = getMaterial();
	if (!mat->isLoaded()) 
	{
		mat->load();
	}
	Ogre::Technique* tech = mat->getBestTechnique();
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

		// If inside light geometry, render back faces with CMPF_GREATER, otherwise normally
		if (mParentLight->getType() == Ogre::Light::LT_DIRECTIONAL)
		{
			pass->setCullingMode(Ogre::CULL_CLOCKWISE);
			pass->setDepthCheckEnabled(false);
		}
		else
		{
			pass->setDepthCheckEnabled(true);
			if (isCameraInsideLight(camera))
			{
				pass->setCullingMode(Ogre::CULL_ANTICLOCKWISE);
				pass->setDepthFunction(Ogre::CMPF_GREATER_EQUAL);
			}
			else
			{
				pass->setCullingMode(Ogre::CULL_CLOCKWISE);
				pass->setDepthFunction(Ogre::CMPF_LESS_EQUAL);
			}
		}

		Ogre::Camera shadowCam("ShadowCameraSetupCam", 0);
		shadowCam._notifyViewport(camera->getViewport());
		Ogre::SceneManager* sm = mParentLight->_getManager();
		sm->getShadowCameraSetup()->getShadowCamera(sm, 
			camera, camera->getViewport(), mParentLight, &shadowCam, 0);
			
		//Get the shadow camera position
		if (params->_findNamedConstantDefinition("shadowCamPos")) 
		{
			params->setNamedConstant("shadowCamPos", shadowCam.getPosition());
		}
		if (params->_findNamedConstantDefinition("shadowFarClip"))
		{
			params->setNamedConstant("shadowFarClip", shadowCam.getFarClipDistance());
		}

	}
}
