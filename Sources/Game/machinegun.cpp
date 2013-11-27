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
	
	Vector3 canonLenght = Vector3(0, 0, -0.5);
	
	Vector3 bulletLocation = mShip->getGlobalPosition(getLocation() + canonLenght);
	Vector3 bulletVelocity = mShip->getSpeed() +  mShip->getRotation() * Vector3(0, 0, -500);
	
	
	Bullet* bullet = new Bullet(mGame, mName + "_Bullet" + StringConverter::toString(bulletId++), this,  bulletLocation, getRotation() * mShip->getRotation(), bulletVelocity );
	
	gameLog("Fire !");
}
