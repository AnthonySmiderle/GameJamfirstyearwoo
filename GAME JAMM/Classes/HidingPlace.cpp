#include "HidingPlace.h"

HidingPlace::HidingPlace(cocos2d::Vec2 start, cocos2d::Vec2 end) : placeToHide(start,end)
{
}

g3nts::PrimitiveRect HidingPlace::getPlaceToHide() const
{
	return placeToHide;
}


