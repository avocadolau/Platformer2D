#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#pragma once

#include "Collider.h"

struct Platform
{
	Platform()
	{

	}
	Collider* col;
	Collider* down;
	bool canFall;
	int timeToFall;
};

#endif // !__PLATFORM_H__
