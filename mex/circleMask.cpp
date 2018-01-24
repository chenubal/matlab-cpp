#include <math.h>
#include "mex.h"
#include <sstream>
#include "geometry.h"

bool checkArgs(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	auto fname = mexFunctionName();
	if (nrhs == 0)
	{
		std::stringstream ss("Function");
		ss << "Function " << fname << "(from " << __DATE__ << ")\n";
		ss << "Usage A = " << fname << "(rows,cols,centerX,centerY,radius);\n";
		mexPrintf(ss.str().c_str()); 
		return false;
	}
	if (nrhs < 5)
	{
		std::string tag("MATLAB:"); tag += fname;
		mexErrMsgIdAndTxt(tag.c_str(), "needs at least five arguments!");
	}
	for (size_t i = 0; i < 5; i++)
	{
		std::string tag("MATLAB:"); tag += fname;
		std::string msg("Invalid argument "); msg += std::to_string(i + 1) + ": must be numeric!";
		if (!mxIsNumeric(prhs[i])) mexErrMsgIdAndTxt(tag.c_str(), msg.c_str());
	}
	return true;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if (!checkArgs(nlhs, plhs, nrhs, prhs)) return;
	// Create boolean 2D array
	mwSize dims[2] = { mwSize(mxGetPr(prhs[0])[0]), mwSize(mxGetPr(prhs[1])[0]) };
	plhs[0] = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
	// Compile calculation data
	JH::Point center{ mxGetPr(prhs[2])[0] - 1, mxGetPr(prhs[3])[0] - 1 };
	double radius = mxGetPr(prhs[4])[0];
	bool *data = (bool*)mxGetData(plhs[0]);
	JH::Circle circle(center, radius);
	// Calculate make values by location
	for (auto i = 0; i < dims[0] * dims[1]; i++)
	{
		JH::Point P{ double(i % dims[0]), double(i / dims[0]) };
		data[i] = circle.contains(P);
	}
}
