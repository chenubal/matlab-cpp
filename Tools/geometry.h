#pragma once

namespace JH
{
	struct Point
	{
		const double x, y;
	};

	struct Circle
	{
		Circle() = delete;
		Circle(Point const& Q, double r) : S(Q), r2(r*r) {}
		bool includes(Point const& Q)
		{
			auto dx = S.x - Q.x;
			auto dy = S.y - Q.y;
			return (dx*dx + dy*dy) <= r2;
		}
		double radius() { return sqrt(r2); }
	private:
		const double r2;
		const Point S;
	};

	struct Ellipse
	{
		Ellipse() = delete;
		Ellipse(Point const& Q, double _ra, double _rb) : S(Q), ra(_ra), rb(_rb) { if (ra <= 0.0 || rb <= 0.0) throw "invalid radii"; }
		bool includes(Point const& Q)
		{
			auto dx = (S.x - Q.x)/ra;
			auto dy = (S.y - Q.y)/rb;
			return (dx*dx + dy*dy) <= 1.0;
		}
	private:
		const double ra,rb;
		const Point S;
	};

}

