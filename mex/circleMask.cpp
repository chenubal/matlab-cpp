#include "ml_tools.h"
#include "geometry.h"
#include <sstream>

struct CP
{
    mwSize rows=0,cols =0;
    double x=0.0,y=0.0;
    double radius = 0.0;
    
};
using optCP = JH::optional<CP>;
optCP checkArgs(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
    using JH::NumericValue;
	auto fname = mexFunctionName();
	if (nrhs == 0)
	{
		std::stringstream ss("Function");
		ss << "Function " << fname << "(from " << __DATE__ << ")\n";
		ss << "Usage A = " << fname << "(rows,cols,centerX,centerY,radius);\n";
		mexPrintf(ss.str().c_str()); 
		return optCP();
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
    CP result{ NumericValue<mwSize>(prhs[0]), NumericValue<mwSize>(prhs[1]),
               NumericValue<>(prhs[2]) - 1,NumericValue<>(prhs[3]) - 1,
               NumericValue<>(prhs[4])};
	return optCP(result);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if ( auto p = checkArgs(nlhs, plhs, nrhs, prhs) )
    {
        auto& arg = p.value; 
        // Create boolean 2D array
        mwSize dims[2] = {  arg.cols, arg.rows };
        plhs[0] = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
        // Compile calculation data
        auto *data = (bool*)mxGetData(plhs[0]);
        JH::Circle circle({arg.y, arg.x }, arg.radius);
        // Calculate make values by location
        auto n = arg.cols * arg.rows ;
        for (auto i = 0; i < n; i++)
        {
            data[i] = circle.contains({ double(i % arg.cols), double(i / arg.cols) });
        }
    }
}
