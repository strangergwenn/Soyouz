/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ACTOR_H_
#define __ACTOR_H_

#include "Engine/game.hpp"
#include "Engine/savable.hpp"
#include "Engine/gametypes.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Actor : public Savable
{

public:
	
	/**
	 * @brief Create an actor
	 * @param g				Game actor
	 * @param name			Unique name to set
	 **/
	Actor(Game* g, String name);
	
	/**
	 * @brief Delete an actor
	 **/
	virtual ~Actor();
	
	/**
	 * @brief Pre-tick event
	 * @param evt			Frame event
	 **/
	virtual void preTick(const Ogre::FrameEvent& evt);
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	virtual void tick(const Ogre::FrameEvent& evt);
	
	/**
	 * @brief Attach something to this actor
	 * @param obj			Attached object
	 **/
	virtual void attachObject(Ogre::MovableObject* obj);
		
	/**
	 * @brief Attach an actor to this actor
	 * @param obj			Attached actor
	 **/
	virtual void attachActor(Actor* obj);

	/**
	 * @brief Set the actor location in the world
	 * @param offset		Location vector
	 **/
	virtual void setLocation(Vector3 newPos);

	/**
	 * @brief Set the actor rotation in the world
	 * @param offset		Rotation vector
	 **/
	virtual void setRotation(Quaternion newRot);

	/**
	 * @brief Set the actor scale
	 * @param scale			New scale
	 **/
	virtual void setScale(float scale);

	/**
	 * @brief Translate the actor in the world
	 * @param offset		Movement vector
	 * @param bRelative		true if relative to the node
	 **/
	virtual void translate(Vector3 offset, bool bRelative = true);
	
	/**
	 * @brief Rotate the actor in the world
	 * @param offset		Rotation vector (pitch, yaw, roll) in degrees
	 **/
	virtual void rotate(Quaternion rotator);

	/**
	 * @brief Get the the actor location in the world
	 * @return the location vector
	 **/
	Vector3 location();

	/**
	 * @brief Get the the actor rotation in the world
	 * @return the rotation vector
	 **/
	Quaternion rotation();
	
	/**
	 * @brief Get the current node
	 * @return the node
	 **/
	Ogre::SceneNode* getNode();


protected:

	/**
	 * @brief Override this to save your class
	 * @brief Call parent if inherited, call setSaveGroup(), then saveValue()
	 **/
	virtual void save();

	/**
	 * @brief Override this to load your class
	 * @brief Call parent if inherited, call setSaveGroup(), then getValue()
	 **/
	virtual void load();

	/**
	 * @brief Override this to set your file name
	 * @return the relative file name
	 **/
	virtual String getFileName();
	
	/**
	 * @brief Parse a string into a quaternion
	 * @param vec			Input string
	 * @return the Ogre quaternion
	 **/
	Quaternion directionFromString(Ogre::String quat);
	
	/**
	 * @brief Write text to the log file
	 * @param text				Input data
	 **/
	void gameLog(String text);
	
	/**
	 * @brief Create a 3D helper and attach it to the actor node
	 **/
	void create3DHelper(void);


protected:

	
	// Render data
	String mName;
	Game* mGame;
	Ogre::SceneNode* mNode;

};

#endif /* __ACTOR_H_ */

