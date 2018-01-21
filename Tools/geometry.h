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
		bool contains(Point const& P) const
		{
			auto Pt = rotate(Center - P) * Point { 1.0 / radiusX, 1.0 / radiusY };
			return std::abs(Pt.x) <= 1.0 && std::abs(Pt.y) <= 1.0;
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
		Circle(Point const& P, double r) : Center(P), radiusSqr(r*r) { if (r <= 0.0) throw "invalid radius"; }

		bool contains(Point const& P) const { return (Center - P).distSqr() <= radiusSqr; }
		double radius() const { return sqrt(radiusSqr); }
		Rectangle box() const { auto r = radius();	return Rectangle(Center, r, r); }
	};

	struct Ellipse
	{
		const Rotator rotate;
		const double radiusX, radiusY;
		const Point Center;

		Ellipse() = delete;
		Ellipse(Point const& P, double rx, double ry, double phi = 0.0) : Center(P), radiusX(rx), radiusY(ry), rotate(phi) { if (radiusX <= 0.0 || radiusY <= 0.0) throw "invalid radii"; }

		bool contains(Point const& P) const
		{
			auto Pt = rotate(Center - P) * Point { 1.0 / radiusX, 1.0 / radiusY };
			return Pt.distSqr() <= 1.0;
		}
		Rectangle box() const
		{
			auto c2 = rotate.c * rotate.c;
			auto s2 = rotate.s * rotate.s;
			auto rx2 = radiusX * radiusX;
			auto ry2 = radiusY * radiusY;
			return Rectangle(Center, sqrt(rx2*c2 + ry2 * s2), sqrt(rx2*s2 + ry2 * c2));
		}
	};
}

