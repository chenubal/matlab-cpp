#pragma once
#include <algorithm> 

namespace JH
{

	struct Point
	{
		Point() = delete;
		Point operator-(Point const& P) const { return { x - P.x, y - P.y }; }
		const double x, y;
	};

	template<class T=double>
	const T pi{ 3.141592653589793238462643};

	struct Rotator
	{
		Rotator() = delete;
		Rotator(double phi): c(cos(phi)), s(sin(phi)) {}
		Point operator()(Point const& P) const { return { c*P.x - s * P.y, P.x*s + c * P.y }; }
		const double c, s;
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
		Circle(Point const& Q, double r) : center(Q), radiusSqr(r*r) { if (r <= 0.0) throw "invalid radius"; }
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
		Ellipse(Point const& Q, double ra, double rb, double phi=-pi<>/4.0) : center(Q), radiusA(ra), radiusB(rb), rot(phi) { if (radiusA <= 0.0 || radiusB <= 0.0) throw "invalid radii"; }
		bool contains(Point const& Q) const
		{
			auto P = rot(center -Q);
			auto dx = P.x/ radiusA;
			auto dy = P.y/ radiusB;
			return (dx*dx + dy*dy) <= 1.0;
		}
	private:
		Rotator rot;
		const double radiusA,radiusB;
		const Point center;
	};

}

