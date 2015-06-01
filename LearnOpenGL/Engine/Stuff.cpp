#include "Stuff.h"
#include <cmath>

const double PI = std::atan(1.0) * 4;

void Stuff::generateNicePosition(float in, float& outx, float& outy)
{
	bool odd = ((int)in % 2) == 1;
	outx = in - std::floor(in);
	outy = sin(outx * 2 * PI) / 2.0f;
	outx -= 0.5;
	if (odd)
	{
		outx *= -1.0f;
	}
}