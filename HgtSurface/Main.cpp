#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "LoadShaders.h"
#include "vmath.h"
#include "Hgt.h"

using namespace std;

int main()
{
	int wait;
	int heightBuffer[ONE_ARC_SIZE * 2];
	float scaleBuffer[ONE_ARC_SIZE * 2];

	Hgt::hgtLoader("N36W112.hgt", heightBuffer, Hgt::ARC_TYPES::ONE_ARC);

	Hgt::scaleHgtBuffer(heightBuffer, Hgt::ARC_TYPES::ONE_ARC, scaleBuffer, 1);

	for (int i = 0; i < ONE_ARC_SIZE * 2; ++i)
	{
		cout << "Height " << i << ": " << heightBuffer[i] << "\n";
		cout << "Scaled " << i << ": " << scaleBuffer[i] << "\n";
	}

	cin >> wait;
    return 0;
}