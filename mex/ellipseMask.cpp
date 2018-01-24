#include "geometry.h"
#include "ml_tools.h"
#include <sstream>

mxArray* checkArgs(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	using JH::CellValue;
	mxArray *result = nullptr;
	auto fname = mexFunctionName();
	if (nrhs == 0)
	{
		std::stringstream ss("Function");
		ss << "Function " << fname << "(from " << __DATE__ << ")\n";
		ss << "Usage: A=" << fname <<"(Size,Center,Radii); where Size, Center and Radii are 2x1 cells\n"
		   << "Example: A=" << fname << "({300,400},{100,100},{10,5});\n";
		mexPrintf(ss.str().c_str());
		return false;
	}
	if (nrhs >= 3)
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

JH::Ellipse createEllipse(int nrhs, const mxArray*prhs[])
{
	using JH::CellValue;
	double phi = 0.0;
	if (nrhs > 3 && mxIsNumeric(prhs[3])) phi = JH::NumericValue<>(prhs[3]);
	auto ra = CellValue<>(prhs[2], 0), rb = CellValue<>(prhs[2], 1);
	return JH::Ellipse({ CellValue<>(prhs[1], 0), CellValue<>(prhs[1], 1) }, ra, rb,phi);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if (auto work = checkArgs(nlhs, plhs, nrhs, prhs))
	{
		using JH::CellValue;
		auto ellipse = createEllipse(nrhs,prhs);
		auto numRows = CellValue<mwSize>(prhs[0], 0);
		auto numData = numRows * CellValue<mwSize>(prhs[0], 1);
		auto i = 0UL;
		for (auto &x : JH::makeProxy((bool*)mxGetData(work),numData))
		{
			x = ellipse.contains({ double(i % numRows), double(i / numRows) });
			i++;
		}
		plhs[0] = work;
	}
}