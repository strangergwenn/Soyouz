/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/game.hpp"
#include "Game/ship.hpp"
#include "Game/thruster.hpp"
#include "Game/machinegun.hpp"


/*----------------------------------------------
	Definitions
----------------------------------------------*/

const float MAX_SPEED_RATIO =	+1.0f;
const float MIN_SPEED_RATIO =	-1.0f;

const Ogre::String TEMPLATE_SHIP_DIR = "Ships/";


/*----------------------------------------------
	Constructor
----------------------------------------------*/
	
Ship::Ship(Game* g, Ogre::String name, Ogre::String templateFile)
	: MeshActor(g, name)
{
	// Steering controls
	mSteerX = 0;
	mSteerY = 0;
	mSteerRoll = 0;
	mSpeed = 0;
	
	// Load template file
	loadTemplate(TEMPLATE_SHIP_DIR + templateFile + ".xml");

	// Speed limits
	setTemplateGroup("steering");
	mMaxSpeed = loadFloatValue("linearSpeedLimit");
	mMaxAngularSpeed = loadFloatValue("angularSpeedLimit");
	mSoftModeLimit = loadFloatValue("linearSoftModeLimit");
	mSoftModeAngularLimit = loadFloatValue("angularSoftModeLimit");

	// Hull setup
	setTemplateGroup("hull");
	setModel(loadStringValue("mesh") + Ogre::String(".mesh"), loadFloatValue("mass"));
	setMaterial(loadStringValue("material"));

	// Bonus data
	setTemplateGroup("description");
	mShipSize = loadIntValue("size");
	mShipClass = loadStringValue("class");
	mShipType = loadStringValue("type");
	mShipStory = loadStringValue("story");

	// External content
	setupEngines();
	setupWeapons();
	setupAddons();
	closeTemplate();
}


Ship::~Ship()
{
}


/*----------------------------------------------
	Content
----------------------------------------------*/

void Ship::preTick(const Ogre::FrameEvent& evt)
{
	clearForces();
}


void Ship::tick(const Ogre::FrameEvent& evt)
{
	Vector3 localSpeed = getLocalSpeed();
	Vector3 localAngularSpeed = getLocalAngularSpeed();

	mCommandRotator.x = computeSoftAngularCommand(localAngularSpeed.x, mSteerY);
	mCommandRotator.y = computeSoftAngularCommand(localAngularSpeed.y, -mSteerX);
	mCommandRotator.z = computeSoftAngularCommand(localAngularSpeed.z, mSteerRoll);

	mCommandVector.x = computeSoftLinearCommand(localSpeed.x, 0.0f);
	mCommandVector.y = computeSoftLinearCommand(localSpeed.y, 0.0f);
	mCommandVector.z = computeSoftLinearCommand(localSpeed.z, mSpeed);

	MeshActor::tick(evt);
}


/*----------------------------------------------
	Content
----------------------------------------------*/

void Ship::setupEngines()
{
	setTemplateGroup("engines");
	tinyxml2::XMLElement* i = mSaveGroup->FirstChildElement("engine");

	while (i != NULL)
	{
		// Parse data
		Vector3 engineLocation = Ogre::StringConverter::parseVector3(i->Attribute("location"));
		Quaternion engineRotation = directionFromString(i->Attribute("direction"));

		// Add a thruster
		if (i->Attribute("type") == Ogre::String("thruster"))
		{
			Thruster* engine = new Thruster(mGame, mName + "_Eng" + StringConverter::toString(mThrusters.size()), this, engineLocation, engineRotation);
			mThrusters.push_back(engine);
		}

		// Add an engine
		else
		{
			MeshActor* mainengine = new MeshActor(mGame, mName + "mainengine", "SM_Engine_2_Mod1.mesh", "MI_Engine_2_Mod1");
			mainengine->rotate(engineRotation);
			mainengine->setLocation(engineLocation);
			this->attachActor(mainengine);
		}
		i = i->NextSiblingElement("engine");
	}
}


void Ship::setupWeapons()
{
	Weapon* weapon = new MachineGun(
		mGame,
		mName + "_Weapon" + StringConverter::toString(mWeapons.size()),
		this,
		Vector3(0, 0, -8),
		Quaternion(Radian(Degree(  0).valueRadians()), Vector3(0,1,0))
	);
	mWeapons.push_back(weapon);
}


void Ship::setupAddons()
{
}


/*----------------------------------------------
	Save
----------------------------------------------*/

bool Ship::isSavable()
{
	return true;
}


/*----------------------------------------------
	Weapons
----------------------------------------------*/

void Ship::setFireOrder(bool fire)
{
	if (mWeapons.size() > 0)
	{
		mWeapons.front()->setFireOrder(fire);
	}
}

Weapon* Ship::getPrimaryWeapon()
{
	if (mWeapons.size() > 0)
	{
		return mWeapons.front();
	}
	return NULL;
}

/*----------------------------------------------
	Steering
----------------------------------------------*/

float Ship::computeSoftLinearCommand(float measure, float command)
{
	float result;
	
	if (measure + (command * mMaxSpeed) < -mSoftModeLimit)
	{
		result = 1;
	}
	else if (measure + (command * mMaxSpeed) > mSoftModeLimit )
	{
		result = -1;
	}
	else
	{
		result =  - (1.0f / mSoftModeLimit) * (measure + (command * mMaxSpeed));
	}

	return result;
}


float Ship::computeSoftAngularCommand(float measure, float command)
{
	float result;

	if (measure - (command * mMaxAngularSpeed) < -mSoftModeAngularLimit)
	{
		result = 1;
	}
	else if (measure - (command * mMaxAngularSpeed) > mSoftModeAngularLimit)
	{
		result = -1;
	}
	else
	{
		result = - (1.0f / mSoftModeAngularLimit) * (measure - (command * mMaxAngularSpeed));
	}

	return result;
}


/*----------------------------------------------
	Getters / Setters
----------------------------------------------*/

void Ship::setSpeed(float speed)
{
	mSpeed = Math::Clamp(speed, MIN_SPEED_RATIO, MAX_SPEED_RATIO);
}


void Ship::setSteer(float x, float y)
{
	mSteerX = Math::Clamp(x, -1.f, 1.f);
	mSteerY = Math::Clamp(y, -1.f, 1.f);
}


void Ship::setRoll(float roll)
{
	mSteerRoll = Math::Clamp(roll, -20.0f, 20.0f);
}


Vector3 Ship::getDirectionCommand(void)
{
	return mCommandVector;
}
	

Vector3 Ship::getRotationCommand(void)
{
	return mCommandRotator;
}


float Ship::getMaxSpeed(void)
{
	return mMaxSpeed;
}
	

float Ship::getMaxAngularSpeed(void)
{
	return mMaxAngularSpeed;
}
