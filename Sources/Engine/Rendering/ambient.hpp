/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef _AMBIENT_H
#define _AMBIENT_H


#include "Engine/Rendering/renderer.hpp"


/*----------------------------------------------
	Ambient lighting
----------------------------------------------*/

class AmbientLight : public Ogre::SimpleRenderable
{

public:

	/**
	 * @brief Ambient light constructor
	 **/
	AmbientLight();

	/**
	 * @brief Ambient light destructor
	 **/
	~AmbientLight();
	
	/**
	 * @brief Get the light radius
	 * @return the max effective distance
	 **/
	virtual Ogre::Real getBoundingRadius(void) const;
	
	/**
	 * @brief Get the view depth (zero)
	 * @param cam				Ignored
	 * @return zero
	 **/
	virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
	
	/**
	 * @brief Get the current material
	 * @return a pointer to the material
	 **/
	virtual const Ogre::MaterialPtr& getMaterial(void) const;
	
	/**
	 * @brief Get the world transformation
	 * @param xform				Transformation matrix to build
	 **/
	virtual void getWorldTransforms(Ogre::Matrix4* xform) const;
	
	/**
	 * @brief Update the lighting from the camera position
	 * @param camera			Camera to use
	 **/
	void updateFromCamera(Ogre::Camera* camera);


protected:

	// Light data
	Ogre::Real mRadius;
	Ogre::MaterialPtr mMatPtr;

};
 

#endif
