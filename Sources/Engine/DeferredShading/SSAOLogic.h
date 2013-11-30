
#ifndef _SSAOLOGIC_H
#define _SSAOLOGIC_H

#include "ListenerFactoryLogic.h"

class SSAOLogic : public ListenerFactoryLogic
{
protected:
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
};

#endif
