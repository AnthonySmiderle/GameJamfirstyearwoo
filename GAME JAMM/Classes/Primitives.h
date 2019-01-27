/* Name: Daniel Presas
 * ID: 100699431
 * 
 * Primitives.h - Extends all of the Cocos primitive drawing classes, and adds a capsule primitive as well
*/

#pragma once

#include "cocos2d.h"

namespace g3nts {
	class PrimitiveRect {
	public:
		// CONSTRUCTORS/DECONSTRUCTORS
		PrimitiveRect();
		PrimitiveRect(const cocos2d::Vec2& startPos,
			const cocos2d::Vec2& endPos,
			const cocos2d::Color4F& colour);
		PrimitiveRect(const cocos2d::Vec2& startPos,
			const cocos2d::Vec2& endPos);

		// MEMBER FUNCTIONS
		void redraw();

		cocos2d::DrawNode* getNode() const;
		cocos2d::Vec2 getStartPosition() const;
		cocos2d::Vec2 getEndPosition() const;
		cocos2d::Vec2 getCentrePosition() const;
		
		float getWidth();
		float getHeight();

		void setNewPositions(cocos2d::Vec2& startPos, cocos2d::Vec2& endPos);

	private:
		cocos2d::DrawNode* _node;
		cocos2d::Vec2 _startPos, _endPos;
		cocos2d::Color4F _colour;

	};

	class PrimitiveCircle {
	public:
		// CONSTRUCTORS/DECONSTRUCTORS
		PrimitiveCircle();
		PrimitiveCircle(const cocos2d::Vec2& centrePos,
						const float radius,
						const float angle,
						const unsigned int segments,
						const bool drawRadius,
						const cocos2d::Color4F& colour);
		PrimitiveCircle(const cocos2d::Vec2& centrePos,
						const float radius,
						const cocos2d::Color4F& colour);
		PrimitiveCircle(const cocos2d::Vec2& centrePos,
						const float radius);

		// MEMBER FUNCTIONS
		void redraw();

		cocos2d::DrawNode* getNode() const;
		cocos2d::Vec2 getPosition() const;
		float getRadius() const;

		void setPosition(cocos2d::Vec2& position);

	private:
		cocos2d::DrawNode* _node;
		cocos2d::Vec2 _position;
		float _radius;
		float _angle;
		unsigned int _segments;
		cocos2d::Color4F _colour;
	};

	class PrimitiveLine {
	public:
		// CONSTRUCTORS/DECONSTRUCTORS
		PrimitiveLine();
		PrimitiveLine(const cocos2d::Vec2& startPos,
					  const cocos2d::Vec2& endPos,
					  const cocos2d::Color4F& colour);
		PrimitiveLine(const cocos2d::Vec2& startPos,
					  const cocos2d::Vec2& endPos);

		// MEMBER FUNCTIONS
		cocos2d::DrawNode* getNode() const;

	private:
		cocos2d::DrawNode* _node;

	};

	class PrimitiveCapsule {
	public:
		// CONSTRUCTORS/DECONSTRUCTORS
		PrimitiveCapsule();
		PrimitiveCapsule(const cocos2d::Vec2& startPos,
						 const cocos2d::Vec2& endPos,
						 const float radius, 
						 const cocos2d::Color4F& colour);
		PrimitiveCapsule(const cocos2d::Vec2& startPos,
						 const cocos2d::Vec2& endPos,
						 const float radius);

		// MEMBER FUNCTIONS
		cocos2d::DrawNode* getNode() const;

	private:
		cocos2d::DrawNode* _node;

	};

	bool isColliding(PrimitiveCircle& c1, PrimitiveCircle& c2);
	bool isColliding(PrimitiveRect& r1, PrimitiveCircle& c2);
	

}