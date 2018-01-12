#include <math.h>
#include "mex.h"
#include "../Tools/geometry.h"

bool checkargs(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	auto fname = mexFunctionName();
	mexPrintf("Function %s\n", fname);
	if (nrhs >0)
	{
		if (mxIsCell(prhs[0]))
		{
			if (mxGetN(prhs[0]) == 2)
			{
				auto x1 = mxGetPr(mxGetCell(prhs[0], 0))[0];
				auto x2 = mxGetPr(mxGetCell(prhs[0], 1))[0];
				mexPrintf("Cell size {%f, %f}\n", x1,x2);
			}

		}
	}
	return false;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if (!checkargs(nlhs, plhs, nrhs, prhs)) return;
}