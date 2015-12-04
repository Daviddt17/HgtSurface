#include <iostream>
#include "Hgt.h"

using namespace std;

int main()
{
	int wait;
	int heightBuffer[ONE_ARC_SIZE * 2];

	Hgt::HgtLoader("N36W112.hgt", heightBuffer, Hgt::ARC_TYPES::ONE_ARC);

	for (int i = 0; i < ONE_ARC_SIZE * 2; ++i)
	{
		cout << "Height " << i << ": " << heightBuffer[i] << "\n";
	}

	cin >> wait;
    return 0;
}

