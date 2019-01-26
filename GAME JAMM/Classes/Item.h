#pragma once
#include "Primitive.h"
namespace Pm {
	class Item{
	public:
		Item(Pm::CirclePrimitive& BOUNDCIRCLE);
		

		Pm::CirclePrimitive getBoundCircle() ;

	private:
		Pm::CirclePrimitive boundCircle;
		//cocos2d::Sprite itemSprite;

	};
}