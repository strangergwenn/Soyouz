/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __TEST_H_
#define __TEST_H_

#include "Engine/game.hpp"
#include "Engine/meshactor.hpp"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class TestWorld : public Game
{

public:
	
	void construct();
	void destruct();
	void setupPlayer();
	
};

#endif /* __TEST_H_ */
