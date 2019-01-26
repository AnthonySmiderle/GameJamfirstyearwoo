#include "Item.h"

Pm::Item::Item(Pm::CirclePrimitive& BOUNDCIRCLE)
{
	boundCircle = BOUNDCIRCLE;
}



Pm::CirclePrimitive Pm::Item::getBoundCircle()
{
	return boundCircle;
}
