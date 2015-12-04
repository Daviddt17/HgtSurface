#include "Hgt.h"

int Hgt::HgtLoader(const char* fileName, int heightBuffer[], int hgtArcType)
{
	FILE *hgtFile;
	int heightCount = 0;
	int bufferSize = 0;

	// Open file
	hgtFile = fopen(fileName, "rb");

	if (hgtFile == NULL)
	{
		return 1;
	}

	// Determine buffer size
	if (hgtArcType == Hgt::ARC_TYPES::ONE_ARC)
	{
		bufferSize = 2 * ONE_ARC_SIZE;
	}
	else
	{
		bufferSize = 2 * THREE_ARC_SIZE;
	}

	// Load Hgt data
	while (!feof(hgtFile))
	{
		char charBuffer[2];
		int numberOfCharsRead = fread(charBuffer, 1, 2, hgtFile);

		// Check if we got both bytes
		if (numberOfCharsRead != 2)
		{
			return 2;
		}

		// Check if we've filled the buffer to spec
		if (heightCount + 1 > bufferSize)
		{
			return 0;
		}

		heightBuffer[heightCount] = (charBuffer[0] << 8) | charBuffer[1];
		heightCount++;
	}
}
