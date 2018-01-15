#include <math.h>
#include "mex.h"
#include "../Tools/geometry.h"
#include <vector>

namespace JH
{
	template<class T>
	struct arrayProxy
	{
		arrayProxy() = delete;
		arrayProxy(T* pData, size_t n) : data(pData), size(n) {}
		T* begin() { return data; }
		T* end() { return data+size; }
		T *data;
		size_t size;
	};
	template<class T > 
	arrayProxy<T> makeProxy(T* pData, size_t n) { return arrayProxy<T>((T*)pData, n); }

	template<class T=double, int M=0>
	T CellValue(mxArray const* pArray, mwSize n) { return T(mxGetPr(mxGetCell(pArray, n))[M]); }
}

mxArray* checkArgs(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	using JH::CellValue;
	mxArray *result = nullptr;
	std::vector<JH::Point> ps;
	ps.push_back({ 10,20 });
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

JH::Ellipse createEllipse( const mxArray*prhs[])
{
	using JH::CellValue;
	auto ra = CellValue<>(prhs[2], 0), rb = CellValue<>(prhs[2], 1);
	return JH::Ellipse({ CellValue<>(prhs[1], 0), CellValue<>(prhs[1], 1) }, ra, rb);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
	if (auto work = checkArgs(nlhs, plhs, nrhs, prhs))
	{
		using JH::CellValue;
		auto ellipse = createEllipse(prhs);
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
	else
	{
		auto fname = mexFunctionName();
		mexPrintf("Usage: A=%s(Size,Center,Radii); where Size, Center and Radii are 2x1 cells\n"\
			       "Example: A=%s({300,400},{100,100},{10,5});\n", fname,fname);
	}
}