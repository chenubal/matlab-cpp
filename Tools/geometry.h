#pragma once
#include <algorithm> 

namespace JH
{
	struct Point
	{
		Point() = delete;
		const double x, y;
	};

	struct Rectangle
	{
		Rectangle() = delete;
		Rectangle(Point const& P, Point const&Q) : topLeft{ std::min(P.x,Q.x), std::min(P.y, Q.y) }, bottomRight({ std::max(P.x, Q.x), std::max(P.y, Q.y) }) {}
		bool contains(Point const& Q) const
		{
			return topLeft.x <= Q.x && Q.x <= topLeft.x && topLeft.y <= Q.y && Q.y <= topLeft.y;
		}
		double width() const { return bottomRight.x - topLeft.x; }
		double height() const { return bottomRight.y - topLeft.y; }
	private:
		const Point topLeft,bottomRight;
	};

	struct Circle
	{
		Circle() = delete;
		Circle(Point const& Q, double r) : center(Q), radiusSqr(r*r) {}
		bool contains(Point const& Q) const
		{
			auto dx = center.x - Q.x;
			auto dy = center.y - Q.y;
			return (dx*dx + dy*dy) <= radiusSqr;
		}
		double radius() const { return sqrt(radiusSqr); }
	private:
		const double radiusSqr;
		const Point center;
	};

	struct Ellipse
	{
		Ellipse() = delete;
		Ellipse(Point const& Q, double ra, double rb) : center(Q), radiusA(ra), radiusB(rb) { if (radiusA <= 0.0 || radiusB <= 0.0) throw "invalid radii"; }
		bool contains(Point const& Q) const
		{
			auto dx = (center.x - Q.x)/ radiusA;
			auto dy = (center.y - Q.y)/ radiusB;
			return (dx*dx + dy*dy) <= 1.0;
		}
	private:
		const double radiusA,radiusB;
		const Point center;
	};

}

