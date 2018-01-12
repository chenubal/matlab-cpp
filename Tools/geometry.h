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
	private:
		double r2;
		Point S;
	};

	struct Ellipse
	{
		Ellipse() = delete;
		Ellipse(Point const& Q, double _ra, double _rb) : S(Q), ra(_ra),rb(_rb) {}
		bool includes(Point const& Q)
		{
			auto dx = (S.x - Q.x)/ra;
			auto dy = (S.y - Q.y)/rb;
			return (dx*dx + dy*dy) <= 1.0;
		}
	private:
		double ra,rb;
		Point S;
	};

}

