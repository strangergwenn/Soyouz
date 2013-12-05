
#ifndef _AMBIENTLIGHT_H
#define _AMBIENTLIGHT_H

#include "Engine/Rendering/renderer.hpp"

class AmbientLight : public Ogre::SimpleRenderable

{
public:
	AmbientLight();
	~AmbientLight();

	/** @copydoc MovableObject::getBoundingRadius */
	virtual Ogre::Real getBoundingRadius(void) const;
	/** @copydoc Renderable::getSquaredViewDepth */
	virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera*) const;
	/** @copydoc Renderable::getMaterial */
	virtual const Ogre::MaterialPtr& getMaterial(void) const;

	virtual void getWorldTransforms(Ogre::Matrix4* xform) const;

	void updateFromCamera(Ogre::Camera* camera);
protected:
	Ogre::Real mRadius;
	Ogre::MaterialPtr mMatPtr;
};
 
#endif
