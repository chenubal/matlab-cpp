#include <math.h>
#include "mex.h"
#include "../Tools/geometry.h"

namespace JH
{
	template<class T=double, int M=0>
	T CellValue(mxArray const* pArray, mwSize n) { return T(mxGetPr(mxGetCell(pArray, n))[M]); }
}
mxArray* checkargs(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	using namespace JH;
	mxArray *result = nullptr;
	if (nrhs == 3)
	{
		auto const S = prhs[0];
		auto const P = prhs[1];
		auto const R = prhs[2];
		if (mxIsCell(S) && mxIsCell(P) && mxGetN(P) && mxGetN(S) >= 2 && mxGetN(P) >= 2 && mxGetN(R) >= 2)
		{
			auto n = CellValue<mwSize>(S, 0);
			auto m = CellValue<mwSize>(S, 1);
			mwSize dims[2] = { n,m };
			result = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
		}
	}
	return result;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if (auto work = checkargs(nlhs, plhs, nrhs, prhs))
	{
		plhs[0] = work;
	}
	else
	{
		auto fname = mexFunctionName();
		mexPrintf("Usage: A=%s(Size,Center,Radii); where Size, Center and Radii are 2x1 cells\n"\
			       "Example: A=%s({300,400},{100,100},{10,5});\n", fname,fname);
	}
}