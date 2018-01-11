#include <math.h>
#include "mex.h"

#define DllExport   __declspec( dllexport )  

namespace jh
{
	struct point
	{
		double x, y;
	};

	struct circle
	{
		circle(point const& Q, double r) : P(Q), r2(r*r) {}
		bool includes(point const& Q)
		{
			auto dx = Q.x - P.x;
			auto dy = Q.y - P.y;
			return (dx*dx + dy*dy) <= r2;
		}
	private:
		double r2;
		point P;
	};
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if (nrhs != 5)  mexErrMsgIdAndTxt("", "Needs five arguments");
	if (!mxIsNumeric(prhs[0])) mexErrMsgIdAndTxt("MATLAB:maskCircle:invalidM", "Invalid arg 1: Must be numeric");
	if (!mxIsNumeric(prhs[1])) mexErrMsgIdAndTxt("MATLAB:maskCircle:invalidN", "Invalid arg 2: Must be numeric");

	/* Check for proper number of arguments */

	double m = mxGetPr(prhs[0])[0];
	double n = mxGetPr(prhs[1])[0];
	double px = mxGetPr(prhs[2])[0] - 1;
	double py = mxGetPr(prhs[3])[0] - 1;
	double r = mxGetPr(prhs[4])[0];
	mwSize dims[2] = { (mwSize)m, (mwSize)n };
	plhs[0] = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
	bool *data = (bool*)mxGetData(plhs[0]);
	mwSize N = dims[0];
	mwSize k = dims[1] * N;
	jh::circle c({ px, py }, r);
	for (auto i = 0; i < k; i++)
	{
		double x = i%N, y = i / N;
		data[i] = c.includes({ x, y });
	}
}
