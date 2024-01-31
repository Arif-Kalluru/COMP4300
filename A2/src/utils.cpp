#include "utils.h"
#include <cstdlib>

int randomInRange(int lower, int upper)
{
	return lower + rand() % (upper - lower + 1);
}