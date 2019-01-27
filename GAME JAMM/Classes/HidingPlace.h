#pragma once
#include "Primitives.h"


class HidingPlace {

public:
	HidingPlace(cocos2d::Vec2 start, cocos2d::Vec2 end);

	

	g3nts::PrimitiveRect getPlaceToHide() const;

private:
	g3nts::PrimitiveRect placeToHide;


};