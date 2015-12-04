#pragma once

#include <iostream>

#define ONE_ARC_SIZE 1201
#define THREE_ARC_SIZE 3601

namespace Hgt 
{
	enum ARC_TYPES
	{
		ONE_ARC, THREE_ARC
	};

	int HgtLoader(const char* fileName, int heightBuffer[], int hgtArcType);
};
