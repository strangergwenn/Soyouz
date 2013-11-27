/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Frédéric Bertolus
**/

#ifndef __WEAPON_MACHINEGUN_H_
#define __WEAPON_MACHINEGUN_H_

#include "Game/weapon.hpp"

/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class MachineGun : public Weapon
{

public:
	
	/**
	 * @brief Create an machinegun weapon
	 * @param g				Game actor
	 * @param name			Unique name to set to the mesh
	 * @param parent		Ship to attach to
	 * @param location		weapon position
	 * @param rotation		weapon rotation
	 **/
	MachineGun(Game* g, String name, Ship* parent, Vector3 location, Quaternion rotation);

protected:
	
	void fire();

};

#endif /* __WEAPON_MACHINEGUN_H_ */
