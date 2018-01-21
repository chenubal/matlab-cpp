#pragma once
#include <algorithm> 

namespace JH
{

	struct Point
	{
		const double x, y;

		Point() = delete;

		double distSqr() const { return x * x + y * y; }
		double dist() const { return sqrt(distSqr()); }
	};

	Point operator-(Point const& P, Point const& Q) { return { P.x - Q.x, P.y - Q.y }; }
	Point operator+(Point const& P, Point const& Q) { return { P.x + Q.x, P.y + Q.y }; }
	Point operator*(Point const& P, Point const& Q) { return { P.x * Q.x, P.y * Q.y }; }
	Point operator*(Point const& P, double a) { return P * Point{a,a}; }

	template<class T = double>
	const T pi{ 3.141592653589793238462643 };

	struct Rotator
	{
		const double c, s;

		Rotator() = delete;
		Rotator(double phi) : c(cos(phi)), s(sin(phi)) {}

		Point operator()(Point const& P) const { return { c * P.x - s * P.y, s * P.x + c * P.y }; }
		double phi() const { return acos(c); }
	};

	struct RectangleSimple
	{
		RectangleSimple() = delete;
		RectangleSimple(Point const& P, Point const&Q) : topLeft{ std::min(P.x,Q.x), std::min(P.y, Q.y) }, bottomRight({ std::max(P.x, Q.x), std::max(P.y, Q.y) }) {}
		bool contains(Point const& Q) const
		{
			return topLeft.x <= Q.x && Q.x <= topLeft.x && topLeft.y <= Q.y && Q.y <= topLeft.y;
		}
		double width() const { return bottomRight.x - topLeft.x; }
		double height() const { return bottomRight.y - topLeft.y; }
	private:
		const Point topLeft, bottomRight;
	};

	struct Rectangle
	{
		const double radiusX, radiusY;
		const Point Center;
		const Rotator rotate;

		Rectangle() = delete;
		Rectangle(Point const& C, double rx, double ry, double phi = 0.0) : Center(C), radiusX(rx), radiusY(ry), rotate(phi) { }
		Rectangle(Point const& P, Point const& Q, double phi = 0.0) :
			Rectangle(P + (P - Q)*0.5, 0.5*std::abs(P.x - Q.x), 0.5*std::abs(P.y - Q.y), phi) { }
		bool contains(Point const& Q) const
		{
			auto P = rotate(Center - Q) * Point { 1.0 / radiusX, 1.0 / radiusY };
			return std::abs(P.x) <= 1.0 && std::abs(P.y) <= 1.0;
		}
		Rectangle box()
		{
			auto d = std::hypot(radiusX, radiusY);
			return Rectangle(Center, d*rotate.c, d*rotate.s);
		}
	};
	struct Circle
	{
		const double radiusSqr;
		const Point Center;

		Circle() = delete;
		Circle(Point const& Q, double r) : Center(Q), radiusSqr(r*r) { if (r <= 0.0) throw "invalid radius"; }

		bool contains(Point const& Q) const { return (Center - Q).distSqr() <= radiusSqr; }
		double radius() const { return sqrt(radiusSqr); }
		Rectangle box() const { auto r = radius();	return Rectangle(Center, r, r); }
	};

	struct Ellipse
	{
		const Rotator rotate;
		const double radiusX, radiusY;
		const Point Center;

		Ellipse() = delete;
		Ellipse(Point const& Q, double rx, double ry, double phi = -pi<> / 4.0) : Center(Q), radiusX(rx), radiusY(ry), rotate(phi) { if (radiusX <= 0.0 || radiusY <= 0.0) throw "invalid radii"; }

		bool contains(Point const& Q) const
		{
			auto P = rotate(Center - Q)* Point { 1.0 / radiusX, 1.0 / radiusY };
			return P.distSqr() <= 1.0;
		}
		Rectangle box()
		{
			auto d = std::hypot(radiusX, radiusY);
			return Rectangle(Center, d*rotate.c, d*rotate.s);
		}
	};

}

