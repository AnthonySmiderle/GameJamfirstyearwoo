//Anthony Smiderle
//100695532
#include "Primitive.h"
#include "2d/CCDrawNode.h"
#include <cmath>

namespace Pm {

	//Square
	Pm::SquarePrimitive::SquarePrimitive(const cocos2d::Vec2&  startingPosition,const cocos2d::Vec2& endPosition)
		: Node(cocos2d::DrawNode::create())//initialize draw node
	{
		start = startingPosition;
		end = endPosition;
		//draw a rectangle given dimensions
		Node->drawRect(start, end, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 1.0f));
	}

	Pm::SquarePrimitive::SquarePrimitive()
	{
	}

	Pm::SquarePrimitive::~SquarePrimitive()
	{
		Node->release();//destroy draw node
	}

	cocos2d::DrawNode * Pm::SquarePrimitive::getDrawNode() const
	{
		return Node;
	}

	void Pm::SquarePrimitive::setPoint(cocos2d::Vec2 & newPosition1, cocos2d::Vec2 & newPosition2)
	{
		start = newPosition1;
		end = newPosition2;
	}

	cocos2d::Vec2 SquarePrimitive::getStart() const
	{
		return start;
	}

	cocos2d::Vec2 SquarePrimitive::getEnd() const
	{
		return end;
	}

	void SquarePrimitive::update()
	{
		Node->drawRect(start, end, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 1.0f));
	}

	bool SquarePrimitive::getIsInRoom() const
	{
		return isInRoom;
	}

	void SquarePrimitive::setIsInRoom(bool y)
	{
		isInRoom = y;
	}

	//Circle
	Pm::CirclePrimitive::CirclePrimitive(const cocos2d::Vec2 &LOCATION, float RADIUS, float ANGLE, unsigned int SEGMENTS)
		: Node(cocos2d::DrawNode::create())
	{
		//draw a circle given dimensions
		Node->drawCircle(LOCATION, RADIUS, ANGLE, SEGMENTS, false, cocos2d::Color4F(0.0f, 1.0f, 0.0f, 1.0f));

		location = LOCATION;
		radius = RADIUS;
		angle = ANGLE;
		segments = SEGMENTS;

	}

	Pm::CirclePrimitive::CirclePrimitive()
	{
	}

	Pm::CirclePrimitive::~CirclePrimitive()
	{
		Node->release();
	}

	void CirclePrimitive::update()
	{
		Node->drawCircle(location, radius, angle, segments, false, cocos2d::Color4F(0.0f, 1.0f, 0.0f, 1.0f));
	}

	cocos2d::DrawNode * Pm::CirclePrimitive::getDrawNode() const
	{
		return Node;
	}

	cocos2d::Vec2 CirclePrimitive::getLocation() const
	{
		return location;
	}

	float CirclePrimitive::getRadius() const
	{
		return radius;
	}

	void CirclePrimitive::setLocation(cocos2d::Vec2 & LOCATION)
	{
		location = LOCATION;
	}

	bool CirclePrimitive::getIsHiding() const
	{
		return isHiding;
	}

	void CirclePrimitive::setIsHiding(bool y)
	{
		isHiding = y;
	}

	//Line
	Pm::LinePrimitive::LinePrimitive(cocos2d::Vec2& ORIGIN, cocos2d::Vec2& ENDPOINT)
		: Node(cocos2d::DrawNode::create())
	{
		//draw a line given two points
		Node->drawLine(ORIGIN, ENDPOINT, cocos2d::Color4F(0.0f, 0.0f, 1.0f, 1.0f));
	}

	Pm::LinePrimitive::LinePrimitive()
	{
	}

	Pm::LinePrimitive::~LinePrimitive()
	{
		Node->release();
	}

	cocos2d::DrawNode * Pm::LinePrimitive::getDrawNode() const
	{
		return Node;
	}


	//Capsule
	Pm::Capsule::Capsule(cocos2d::Vec2 & ORIGIN, cocos2d::Vec2 & ENDPOINT, float RADIUS)
		: Node(cocos2d::DrawNode::create())
	{
		//draw two circles, one at the left most point and one at the right most point, given dimensions
		Node->drawCircle(ORIGIN, RADIUS, 5, 20, false, cocos2d::Color4F(10.0f, 10.0f, 10.0f, 1.0f));
		Node->drawCircle(ENDPOINT, RADIUS, 5, 20, false, cocos2d::Color4F(10.0f, 10.0f, 10.0f, 1.0f));

		//top line of the capsule, add the radius to the y coordinate to translate the line up to the proper place
		cocos2d::Vec2 topLineVector1(ORIGIN.x, ORIGIN.y + RADIUS);
		cocos2d::Vec2 topLineVector2(ENDPOINT.x, ENDPOINT.y + RADIUS);

		Node->drawLine(topLineVector1, topLineVector2, cocos2d::Color4F(10.0f, 10.0f, 10.0f, 1.0f));

		//bottom line, subtract y to get to the right poisition
		cocos2d::Vec2 bottomLineVector1(ORIGIN.x, ORIGIN.y - RADIUS);
		cocos2d::Vec2 bottomLineVector2(ENDPOINT.x, ENDPOINT.y - RADIUS);

		Node->drawLine(bottomLineVector1, bottomLineVector2, cocos2d::Color4F(10.0f, 10.0f, 10.0f, 1.0f));

	}

	Pm::Capsule::~Capsule()
	{
		Node->release();
	}

	cocos2d::DrawNode * Pm::Capsule::getDrawNode() const
	{
		return Node;
	}

	

	bool Pm::colliding(CirclePrimitive& c1, CirclePrimitive& c2)
	{

		cocos2d::Vec2 distance;
		distance.x = (c1.getLocation().x) - (c2.getLocation().x);
		distance.y = (c1.getLocation().y) - (c2.getLocation().y);

		float squaredDistance = (distance.x * distance.x) + (distance.y * distance.y);

		float squaredRadii = (c1.getRadius() * c1.getRadius()) + (c2.getRadius() * c2.getRadius());

		if (squaredDistance <= squaredRadii)
			return true;

		return false;
	}

}