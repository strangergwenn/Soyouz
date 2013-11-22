/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/


#ifndef __BULLET_H_
#define __BULLET_H_

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "Engine/ogre.hpp"


/*----------------------------------------------
	Debug drawer
----------------------------------------------*/

class DebugDrawer : public btIDebugDraw
{
 
public:
 
	/**
	 * @brief Constructor
	 * @param node				Scene node
	 * @param world				Bullet world
	 **/
	DebugDrawer(SceneManager* mgr, SceneNode *node, btDynamicsWorld *world)
		: mNode(node), mWorld(world), mDebugOn(true)
	{
		mLineDrawer = mgr->createManualObject("DebugLines");
		SceneNode* myManualObjectNode = mgr->getRootSceneNode()->createChildSceneNode("DebugLinesNode");
		mNode->attachObject(mLineDrawer);
	}
 
	/**
	 * @brief Destructor
	 **/
	~DebugDrawer()
	{
		delete mLineDrawer;
	}
 
	/**
	 * @brief Re-render
	 **/
	void step()
	{
		if (mDebugOn)
		{
			mLineDrawer->clear();
			mLineDrawer->begin("Green", RenderOperation::OT_LINE_LIST); 
			mWorld->debugDrawWorld();
			mLineDrawer->end(); 
			mNode->needUpdate();
		}
		else
		{
			mLineDrawer->clear();
			mNode->needUpdate();
		}
	}
 
	/**
	 * @brief Add a line
	 * @param from				Line start
	 * @param to				Line end
	 * @param color				Color
	 **/
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
	{
		mLineDrawer->position(Vector3(from.x(), from.y(), from.z()));
		mLineDrawer->position(Vector3(to.x(), to.y(), to.z()));
	}
	
	/**
	 * @brief Add a point
	 * @param from				Line start
	 * @param to				Line end
	 * @param color				Color
	 **/
	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{
	}
	
	/**
	 * @brief Write to the log file
	 * @param warningString		Data to save
	 **/
	void reportErrorWarning(const char* warningString)
	{
		LogManager::getSingleton().logMessage(warningString);
	}
 
	/**
	 * @brief Disabled method
	 **/
	void draw3dText(const btVector3& location,const char* textString)
	{
	}
 
	/**
	 * @brief Set the debugging on ?
	 * @param bIsOn			New status
	 **/
	void setDebugMode(int bIsOn)
	{
		mDebugOn = (bIsOn != 0);
		if (!mDebugOn)
		{
			mLineDrawer->clear();
		}
	}
 
	/**
	 * @brief Is the debugging on ?
	 * @return 1 if true
	 **/
	int getDebugMode() const
	{
		return mDebugOn;
	}
 
protected:

	// Render data
	bool mDebugOn;
	SceneNode* mNode;
	btDynamicsWorld* mWorld;
	ManualObject* mLineDrawer;

};

#endif /* __BULLET_H_ */
