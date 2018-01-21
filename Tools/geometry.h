#pragma once
#include <algorithm> 

namespace JH
{

	struct Point
	{
		const double x, y;

		Point() = delete;

		Point operator-(Point const& P) const { return { x - P.x, y - P.y }; }
		Point operator+(Point const& P) const { return { x + P.x, y + P.y }; }
		Point operator*(double a) const { return { x *a, y *a }; }

		Point stretch(double a, double b) const { return { x *a, y *b}; }
		double distSqr() const { return x*x+y*y; }
		double dist() const { return sqrt(dist()); }
	};

	template<class T=double>
	const T pi{ 3.141592653589793238462643};

	struct Rotator
	{
		const double c, s;

		Rotator() = delete;
		Rotator(double phi): c(cos(phi)), s(sin(phi)) {}

		Point operator()(Point const& P) const { return { c*P.x - s * P.y, P.x*s + c * P.y }; }
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
		const Point topLeft,bottomRight;
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
			auto P = rotate(Center - Q);
			auto dx = P.x / radiusX;
			auto dy = P.y / radiusY;
			return std::abs(dx) <= 1.0 && std::abs(dy) <= 1.0;
		}
		Rectangle box() 
		{
			auto phi = rotate.phi();
			auto d = std::hypot(radiusX, radiusY);
			return Rectangle(Center, d*cos(phi), d*sin(phi));
		}
	};
	struct Circle
	{
		const double radiusSqr;
		const Point Center;

		Circle() = delete;
		Circle(Point const& Q, double r) : Center(Q), radiusSqr(r*r) { if (r <= 0.0) throw "invalid radius"; }

		bool contains(Point const& Q) const	{return (Center-Q).distSqr() <= radiusSqr;}
		double radius() const { return sqrt(radiusSqr); }
		Rectangle box() const {auto r = radius();	return Rectangle(Center, r,r);}
	};

	struct Ellipse
	{
		const Rotator rotate;
		const double radiusX, radiusY;
		const Point Center;

		Ellipse() = delete;
		Ellipse(Point const& Q, double ra, double rb, double phi=-pi<>/4.0) : Center(Q), radiusX(ra), radiusY(rb), rotate(phi) { if (radiusX <= 0.0 || radiusY <= 0.0) throw "invalid radii"; }

		bool contains(Point const& Q) const
		{
			auto P = rotate(Center -Q);
			return P.stretch(1.0/radiusX, 1.0/radiusY).dist() <= 1.0;
		}
		Rectangle box()
		{
			auto phi = rotate.phi();
			auto d = std::hypot(radiusX, radiusY);
			return Rectangle(Center, d*cos(phi), d*sin(phi));
		}
	};

}

