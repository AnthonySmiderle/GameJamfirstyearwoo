/* Name: Daniel Presas
 * ID: 100699431
 *
 * Primitives.cpp - Extends all of the Cocos primitive drawing classes, and adds a capsule primitive as well
*/

#include "Primitives.h"

// DEFAULT CONSTRUCTORS
g3nts::PrimitiveRect::PrimitiveRect() {}
g3nts::PrimitiveCircle::PrimitiveCircle() {}
g3nts::PrimitiveLine::PrimitiveLine() {}
g3nts::PrimitiveCapsule::PrimitiveCapsule() {}

// CONSTRUCTORS WITH PARAMETERS
g3nts::PrimitiveRect::PrimitiveRect(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos, const cocos2d::Color4F& colour)
	: _node(cocos2d::DrawNode::create()), _startPos(startPos), _endPos(endPos), _colour(colour) {
	_node->drawRect(startPos, endPos, colour);
}

g3nts::PrimitiveRect::PrimitiveRect(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos)
	: PrimitiveRect(startPos, endPos, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 1.0f)) {}

void g3nts::PrimitiveRect::redraw() {
	_node->clear();
	_node->drawRect(_startPos, _endPos, _colour);
}

void g3nts::PrimitiveRect::setNewPositions(cocos2d::Vec2& startPos, cocos2d::Vec2& endPos) {
	_startPos = startPos;
	_endPos = endPos;
}

cocos2d::Vec2 g3nts::PrimitiveRect::getStartPosition() const { return _startPos; }
cocos2d::Vec2 g3nts::PrimitiveRect::getEndPosition() const { return _endPos; }

cocos2d::Vec2 g3nts::PrimitiveRect::getCentrePosition() const { return _startPos + (_endPos - _startPos) / 2.0f; }

float g3nts::PrimitiveRect::getWidth()  { return _endPos.x - _startPos.x; }
float g3nts::PrimitiveRect::getHeight() { return _endPos.y - _startPos.y; }



g3nts::PrimitiveCircle::PrimitiveCircle(const cocos2d::Vec2& centrePos,	const float radius,	const float angle, const unsigned int segments, const bool drawRadius, const cocos2d::Color4F& colour)
: _node(cocos2d::DrawNode::create()), _position(centrePos), _radius(radius), _angle(angle), _segments(segments), _colour(colour) {
	_node->drawCircle(centrePos, radius, angle, segments, drawRadius, colour);
}

g3nts::PrimitiveCircle::PrimitiveCircle(const cocos2d::Vec2& centrePos, const float radius, const cocos2d::Color4F& colour)
: PrimitiveCircle(centrePos, radius, 1.0f, 20, false, colour) {}

g3nts::PrimitiveCircle::PrimitiveCircle(const cocos2d::Vec2& centrePos, const float radius)
: PrimitiveCircle(centrePos, radius, 1.0f, 20, false, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 1.0f)) {}

void g3nts::PrimitiveCircle::redraw() {
	_node->clear();
	_node->drawCircle(_position, _radius, _angle, _segments, false, _colour);
}
cocos2d::Vec2 g3nts::PrimitiveCircle::getPosition() const { return _position; }
float g3nts::PrimitiveCircle::getRadius() const { return _radius; }

void g3nts::PrimitiveCircle::setPosition(cocos2d::Vec2& position) { _position = position; }



g3nts::PrimitiveLine::PrimitiveLine(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos, const cocos2d::Color4F& colour)
: _node(cocos2d::DrawNode::create()) { _node->drawLine(startPos, endPos, colour); }

g3nts::PrimitiveLine::PrimitiveLine(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos)
: PrimitiveLine(startPos, endPos, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 1.0f)) {}





g3nts::PrimitiveCapsule::PrimitiveCapsule(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos, const float radius, const cocos2d::Color4F& colour)
: _node(cocos2d::DrawNode::create()) {
	cocos2d::Vec2 direction = startPos - endPos;
	cocos2d::Vec2 normal = { direction.y, -direction.x };

	_node->drawCircle(startPos, radius, 1.0f, 50, false, colour);
	_node->drawCircle(endPos, radius, 1.0f, 50, false, colour);
	_node->drawLine(startPos + (normal.getNormalized() * radius), endPos + (normal.getNormalized() * radius), colour);
	_node->drawLine(startPos - (normal.getNormalized() * radius), endPos - (normal.getNormalized() * radius), colour);
}

g3nts::PrimitiveCapsule::PrimitiveCapsule(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos, const float radius)
	: PrimitiveCapsule(startPos, endPos, radius, cocos2d::Color4F(1.0f, 0.0f, 0.0f, 1.0f)) {}






// NODE GETTERS (SO WE CAN ADD TO THE SCREEN)
cocos2d::DrawNode* g3nts::PrimitiveRect::getNode() const    { return _node; }
cocos2d::DrawNode* g3nts::PrimitiveCircle::getNode() const  { return _node; }
cocos2d::DrawNode* g3nts::PrimitiveLine::getNode() const    { return _node; }
cocos2d::DrawNode* g3nts::PrimitiveCapsule::getNode() const { return _node; }




bool g3nts::isColliding(g3nts::PrimitiveCircle& c1, g3nts::PrimitiveCircle& c2){
	cocos2d::Vec2 distance = c1.getPosition() - c2.getPosition();
	float squaredDistance = (distance.x * distance.x) + (distance.y * distance.y);
	float squaredRadii = (c1.getRadius() * c1.getRadius()) + (c2.getRadius() * c2.getRadius());

	if (squaredDistance <= squaredRadii) return true;
	return false;
}

bool g3nts::isColliding(g3nts::PrimitiveRect& r1, g3nts::PrimitiveCircle& c2) {
	cocos2d::Vec2 test;

	if (c2.getPosition().x < r1.getStartPosition().x) test.x = r1.getStartPosition().x;
	else if (c2.getPosition().x > r1.getEndPosition().x) test.x = r1.getEndPosition().x;
	else test.x = c2.getPosition().x;

	if (c2.getPosition().y < r1.getStartPosition().y) test.y = r1.getStartPosition().y;
	else if (c2.getPosition().y > r1.getEndPosition().y) test.y = r1.getEndPosition().y;
	else test.y = c2.getPosition().y;

	float distanceSq = test.getDistanceSq(c2.getPosition());
	return distanceSq <= (c2.getRadius() * c2.getRadius());
}