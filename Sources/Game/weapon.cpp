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

// TODO make a util file
static const Radian TWO_PI(Math::TWO_PI);
static const Radian PI(Math::PI);
static const Radian HALF_PI(Math::HALF_PI);

static Radian normalizeAngle(Radian angle) {
	Radian result = angle;

	while(result > PI) {
		result -= TWO_PI;
	}

	while(result < -PI) {
		result += TWO_PI;
	}

	return result;
}

void Weapon::tick(const Ogre::FrameEvent& evt)
{
	mTimeSinceLastFire += evt.timeSinceLastFrame;

	// Aim to target
	Vector3 localAimDirection = (mShip->getRotation() * getRotation()).Inverse() * mAimDirection;

	Radian firstAngleSpeed = Radian(0.01);
	Radian secondAngleSpeed = Radian(0.01);

	Radian targetFirstAngle =  normalizeAngle(- Math::ATan2(localAimDirection.x, localAimDirection.y));//  + Radian( localAimDirection.y > 0 ? 0 : Math::PI);
	Radian targetSecondAngle = normalizeAngle(- Math::ATan2(sqrt(localAimDirection.x * localAimDirection.x + localAimDirection.y * localAimDirection.y ), localAimDirection.z) + Radian(Math::PI));

	Radian currentFirstAngle = mTurretFirstRotation.getRoll();
	Radian currentSecondAngle = mTurretSecondRotation.getPitch();

	Radian diffFirstAngle = normalizeAngle(targetFirstAngle - currentFirstAngle);
	Radian diffSecondAngle = normalizeAngle(targetSecondAngle - currentSecondAngle);

	Radian nextFirstAngle;
	Radian nextSecondAngle;

	// If turret support negative angle, optimize
	if(Math::Abs(diffFirstAngle) > HALF_PI) {
		// Target too far, inverse turret
		targetFirstAngle = normalizeAngle(targetFirstAngle + PI);
		targetSecondAngle = - targetSecondAngle;

		diffFirstAngle = normalizeAngle(targetFirstAngle - currentFirstAngle);
		diffSecondAngle = normalizeAngle(targetSecondAngle - currentSecondAngle);
	}

	if(Math::Abs(diffFirstAngle) <= firstAngleSpeed) {
		nextFirstAngle = targetFirstAngle;
	} else if(diffFirstAngle > Radian()) {
		nextFirstAngle = currentFirstAngle + firstAngleSpeed;
	} else {
		nextFirstAngle = currentFirstAngle - firstAngleSpeed;
	}

	if(Math::Abs(diffSecondAngle) <= secondAngleSpeed) {
		nextSecondAngle = targetSecondAngle;
	} else if(diffSecondAngle > Radian())  {
		nextSecondAngle = currentSecondAngle + secondAngleSpeed;
	} else {
		nextSecondAngle = currentSecondAngle - secondAngleSpeed;
	}

	mTurretFirstRotation.FromAngleAxis(nextFirstAngle, Vector3(0,0,1));
	mTurretSecondRotation.FromAngleAxis(nextSecondAngle, Vector3(1,0,0));

	//gameLog("mAimDirection=" + StringConverter::toString(mAimDirection));
	//gameLog("localAimDirection=" + StringConverter::toString(localAimDirection));
	//gameLog("currentFirstAngle=" + StringConverter::toString(currentFirstAngle.valueDegrees()));
	//gameLog("currentSecondAngle=" + StringConverter::toString(currentSecondAngle.valueDegrees()));
	//gameLog("targetFirstAngle=" + StringConverter::toString(targetFirstAngle.valueDegrees()));
	//gameLog("targetSecondAngle=" + StringConverter::toString(targetSecondAngle.valueDegrees()));
	//gameLog("targetFirstAngle=" + StringConverter::toString(targetFirstAngle.valueDegrees()));
	//gameLog("targetSecondAngle=" + StringConverter::toString(targetSecondAngle.valueDegrees()));
	//gameLog("nextFirstAngle=" + StringConverter::toString(nextFirstAngle.valueDegrees()));
	//gameLog("nextSecondAngle=" + StringConverter::toString(nextSecondAngle.valueDegrees()));
	//gameLog("diffFirstAngle=" + StringConverter::toString(diffFirstAngle.valueDegrees()));
	//gameLog("diffSecondAngle=" + StringConverter::toString(diffSecondAngle.valueDegrees()));

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

void Weapon::setAimDirection(Vector3 aimDirection)
{
	mAimDirection = aimDirection;
}
