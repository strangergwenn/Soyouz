/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ACTOR_H_
#define __ACTOR_H_

#include "Engine/game.hpp"

class Game;


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Actor
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
	~Actor();
	
	/**
	 * @brief Pre-tick event
	 * @param evt			Frame event
	 **/
	virtual void preTick(const FrameEvent& evt);
	
	/**
	 * @brief Main tick event
	 * @param evt			Frame event
	 **/
	virtual void tick(const FrameEvent& evt);
	
	/**
	 * @brief Attach something to this actor
	 * @param obj			Attached object
	 **/
	virtual void attachObject(MovableObject* obj);
		
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
	virtual void setRotation(Vector3 newRot);

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
	virtual void rotate(Vector3 rotator);

	/**
	 * @brief Get the the actor location in the world
	 * @return the location vector
	 **/
	Vector3 location();

	/**
	 * @brief Get the the actor rotation in the world
	 * @return the rotation vector
	 **/
	Vector3 rotation();
	
	/**
	 * @brief Get the current node
	 * @return the node
	 **/
	SceneNode* getNode();
	
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
	SceneNode* mNode;

};

#endif /* __ACTOR_H_ */

