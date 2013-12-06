/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef H_DEFERREDLIGHT
#define H_DEFERREDLIGHT


#include "Engine/Rendering/renderer.hpp"
#include "Engine/Rendering/lightmaterial.hpp"


/*----------------------------------------------
	Deferred light class with geometry
----------------------------------------------*/

class DeferredLight: public Ogre::SimpleRenderable
{

public:
	
	/**
	 * @brief Deferred light constructor
	 * @param mat			Light material
	 * @param parentLight	Ogre light
	 * @return the light
	 **/
	DeferredLight(LightMaterialGenerator* mat, Ogre::Light* parentLight);
	
	/**
	 * @brief Deferred light destructor
	 **/
	~DeferredLight();
	
	/**
	 * @brief Update with the parent light data
	 **/
	void updateFromParent();
	
	/**
	 * @brief Update with the camera data
	 * @param camera		Player camera
	 **/
	void updateFromCamera(Ogre::Camera* camera);
	
	/**
	 * @brief Do we cast shadows
	 * @return true if shadows
	 **/
	virtual bool getCastShadows() const;
	
	/**
	 * @brief Get the light radius
	 * @return the max effective distance
	 **/
	virtual Ogre::Real getBoundingRadius(void) const;
	
	/**
	 * @brief Get the light depth
	 * @param camera		Player camera
	 * @return the squared depth of the light in the view
	 **/
	virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera* camera) const;
	
	/**
	 * @brief Get the current material
	 * @return a pointer to the material
	 **/
	virtual const Ogre::MaterialPtr& getMaterial(void) const;
	
	/**
	 * @brief Get the world transformation
	 * @param xform			Transformation matrix to build
	 **/
	virtual void getWorldTransforms(Ogre::Matrix4* xform) const;


protected:
	
	/**
	 * @brief Are we inside the light geometry ?
	 * @param camera		Player camera
	 * @retuen true if we are
	 **/
	bool isCameraInsideLight(Ogre::Camera* camera);
	
	/**
	 * @brief Rebuild the light geometry
	 * @param radius		New geometry radius
	 **/
	void rebuildGeometry(float radius);
	
	/**
	 * @brief Create a sphere for the geometry
	 * @param radius		Sphere radius
	 * @param nRings		Sphere rings (Z)
	 * @param nSegments		Sphere segments (circle)
	 **/
	void createSphere(float radius, int nRings, int nSegments);
	
	/**
	 * @brief Create a rectangle for the geometry
	 **/
	void createRectangle2D();
	
	/**
	 * @brief Create a cone for the geometry
	 * @param radius		Cone radius
	 * @param height		Cone height (Z)
	 * @param nVerticesInBase Base segments (circle)
	 **/
	void createCone(float radius, float height, int nVerticesInBase);
	
	/**
	 * @brief Set the light attenuation parameters
	 * @param c				Attenuation param c
	 * @param b				Attenuation param b
	 * @param a				Attenuation param a
	 **/
	void setAttenuation(float c, float b, float a);
	
	/**
	 * @brief Set the specular colour
	 * @param col			New color
	 **/
	void setSpecularColour(const Ogre::ColourValue &col);
	

	// Light data
	Ogre::uint32 mPermutation;
	Ogre::Light* mParentLight;
	LightMaterialGenerator* mGenerator;
	
	// Light settings
	float mRadius;
	bool bIgnoreWorld;
};

#endif
