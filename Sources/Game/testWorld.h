/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __TEST_H_
#define __TEST_H_

#include "Engine/world.h"


/*----------------------------------------------
Class definitions
----------------------------------------------*/

class TestWorld : public World
{

public:
	
	void construct();
	void destruct();
	void setupPlayer();
	
};

#endif /* __TEST_H_ */
