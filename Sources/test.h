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

class Test : public World
{

public:
	
	/**
	 * @brief World definition
	 **/
	void construct();

	/**
	 * @brief World unloading
	 **/
	void destruct();
	
};

#endif /* __TEST_H_ */
