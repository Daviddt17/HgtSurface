#pragma once

#include <iostream>

#define ONE_ARC_SIZE 1201
#define THREE_ARC_SIZE 3601
#define MAX_HEIGHT 32767
#define MIN_HEIGHT -32768

namespace Hgt 
{
	enum ARC_TYPES
	{
		ONE_ARC, THREE_ARC
	};

	int hgtLoader(const char* fileName, int heightBuffer[], int hgtArcType);
	void scaleHgtBuffer(int heightData[], int hgtArcType, float scaledDataBuffer[], int scale);
};
