/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Frédéric BERTOLUS
**/

#ifndef __BULLET_H_
#define __BULLET_H_

#include "Engine/meshactor.hpp"

class Game;
class Weapon;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Bullet : public MeshActor
{

public:

	/**
	 * @brief Create a bullet
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param parent		Weaon to attach to
	 * @param location		Bullet position
	 * @param rotation		Bullet rotation
	 * @param velocity		Bullet linear velocity
	 **/
	Bullet(Game* g, String name, Weapon* parent, Vector3 location, Quaternion rotation, Vector3 velocity);

	virtual ~Bullet();

	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	void tick(const Ogre::FrameEvent& evt);

protected:

	Weapon* mWeapon;
	Real mLifeTime;
	Real mTimeToLive;

};

#endif /* __BULLET_H_ */
