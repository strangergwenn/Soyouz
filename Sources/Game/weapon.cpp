/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Frédéric Bertolus
**/


#include "Game/weapon.hpp"
#include "Game/ship.hpp"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/
	
Weapon::Weapon(Game* g, String name, Ship* parent, Vector3 location, Quaternion rotation)
	: MeshActor(g, name, "", "")
{
	mFiring = false;
	mTimeSinceLastFire = 0;
	mFirerate = 0.05f; // 50 ms or 1200 rpm
	
	// TODO Customization
	
	// Position
	mRelPosition = location;
	mShip = parent;
	rotate(rotation);
	setLocation(location);
	parent->attachActor(this);
}


void Weapon::tick(const Ogre::FrameEvent& evt)
{
	mTimeSinceLastFire += evt.timeSinceLastFrame;
	
	if(mFiring) {
		if(mTimeSinceLastFire >= mFirerate) {
			//Fire
			mTimeSinceLastFire = 0;
			fire();
		}
	}

	MeshActor::tick(evt);
}

void Weapon::setFireOrder(bool fire)
{
	mFiring = fire;
}
