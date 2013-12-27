/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __ORBITSEGMENT_H_
#define __ORBITSEGMENT_H_

#include "Engine/game.hpp"
#include "Engine/meshactor.hpp"


/*----------------------------------------------
Class definition
----------------------------------------------*/

class OrbitSegment : public Game
{
	/**
	 * @brief Level construction
	 **/
	void construct();
	
	/**
	 * @brief Level destruction
	 **/
	void destruct(){}
	
	/**
	 * @brief Setup the player
	 **/
	void setupPlayer();
	
	/**
	 * @brief Main tick event
	 * @param evt				Frame event
	 **/
	void tick(const Ogre::FrameEvent& evt);


protected:

	// Earth mesh
	MeshActor* earth;

};


#endif /* __ORBITSEGMENT_H_ */
