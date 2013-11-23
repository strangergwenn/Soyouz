/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ENGINE_H_
#define __ENGINE_H_

#include "Engine/meshactor.hpp"

class Game;
class Ship;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Engine : public MeshActor
{

public:
	
	/**
	 * @brief Create an engine
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param parent		Ship to attach to
	 * @param location		Engine position
	 * @param rotation		Engine rotation
	 **/
	Engine(Game* g, String name, MeshActor* parent, Vector3 location, Quaternion rotation);
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	void tick(const Ogre::FrameEvent& evt);
	
	/**
	 * @brief Set the current engine output ratio
	 * @param alpha			Ratio (0 - 1)
	 **/
	void setAlpha(float alpha);
	
	/**
	 * @brief Set the current engine output strength
	 * @param strength		New maximum output
	 **/
	void setStrength(float strength);
	
	/**
	 * @brief Set the current engine acceleration factor
	 * @param strength		Rotation ratio, relative to the acceleration ratio
	 **/
	void setRotationRatio(float strength);

protected:

	Ship* mShip;
	
	Vector3 mRelPosition;
	float mRotationRatio;

	float mStrength;
	float mAlpha;

};

#endif /* __ENGINE_H_ */
