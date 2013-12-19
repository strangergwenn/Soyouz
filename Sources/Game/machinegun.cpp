/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Frédéric Bertolus
**/


#include "Game/machinegun.hpp"
#include "Game/ship.hpp"
#include "Game/bullet.hpp"

static int bulletId = 0;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/
	
MachineGun::MachineGun(Game* g, String name, Ship* parent, Vector3 location, Quaternion rotation)
	: Weapon(g, name, parent, location, rotation)
{
}

void MachineGun::fire()
{
	
	
	float bulletSpeed = 600;

	

	Vector3 bulletLocation = mShip->getGlobalPosition(getLocation() + (mTurretFirstOffset + mTurretFirstRotation * (mTurretSecondOffset  + mTurretSecondRotation * mBarrelOffset)));
	Vector3 bulletVelocity = mShip->getSpeed() + mShip->getRotation() * getRotation() * mTurretFirstRotation * mTurretSecondRotation * Vector3(0, 0, - bulletSpeed);

	Quaternion bulletRotation = mShip->getRotation() * getRotation() * mTurretFirstRotation * mTurretSecondRotation;

	Bullet* bullet = new Bullet(mGame, mName + "_Bullet" + StringConverter::toString(bulletId++), this,  bulletLocation, bulletRotation, bulletVelocity );
	
}
