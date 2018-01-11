#include <math.h>
#include "mex.h"

namespace jh
{
	struct Point
	{
		const double x, y;
	};

	struct Circle
	{
		Circle() = delete;
		Circle(Point const& Q, double r) : P(Q), r2(r*r) {}
		bool includes(Point const& Q)
		{
			auto dx = Q.x - P.x;
			auto dy = Q.y - P.y;
			return (dx*dx + dy*dy) <= r2;
		}
	private:
		double r2;
		Point P;
	};
}
void checkargs(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if (nrhs < 5)  mexErrMsgIdAndTxt("MATLAB:makeCircle", "Needs five arguments");
	if (!mxIsNumeric(prhs[0])) mexErrMsgIdAndTxt("MATLAB:makeCircle", "Invalid arg 1: Must be numeric");
	if (!mxIsNumeric(prhs[1])) mexErrMsgIdAndTxt("MATLAB:makeCircle", "Invalid arg 2: Must be numeric");
	if (!mxIsNumeric(prhs[2])) mexErrMsgIdAndTxt("MATLAB:makeCircle", "Invalid arg 3: Must be numeric");
	if (!mxIsNumeric(prhs[3])) mexErrMsgIdAndTxt("MATLAB:makeCircle", "Invalid arg 4: Must be numeric");
	if (!mxIsNumeric(prhs[4])) mexErrMsgIdAndTxt("MATLAB:makeCircle", "Invalid arg 5: Must be numeric");
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	checkargs(nlhs, plhs, nrhs, prhs);
	// Create boolean 2D array
	mwSize dims[2] = { mwSize(mxGetPr(prhs[0])[0]), mwSize(mxGetPr(prhs[1])[0]) };
	plhs[0] = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
	// Compile calculation data
	jh::Point center{ mxGetPr(prhs[2])[0] - 1, mxGetPr(prhs[3])[0] - 1 };
	double radius = mxGetPr(prhs[4])[0];
	bool *data = (bool*)mxGetData(plhs[0]);
	jh::Circle circle(center, radius);
	// Calculate make values by location
	for (auto i = 0; i <  dims[0] * dims[1]; i++)
	{
		jh::Point P{ double(i % dims[0]), double(i / dims[0]) };
		data[i] = circle.includes(P);
	}
}
