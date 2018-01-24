#pragma once
#include "mex.h"

namespace JH
{
	template<class T>
	struct arrayProxy
	{
		arrayProxy() = delete;
		arrayProxy(T* pData, size_t n) : data(pData), size(n) {}
		T* begin() { return data; }
		T* end() { return data + size; }
		T *data;
		size_t size;
	};
	template<class T >
	arrayProxy<T> makeProxy(T* pData, size_t n) { return arrayProxy<T>((T*)pData, n); }

	template<class T = double, int M = 0>
	T CellValue(mxArray const* pArray, mwSize n) { return T(mxGetPr(mxGetCell(pArray, n))[M]); }

	template<class T = double, int M = 0>
	T NumericValue(mxArray const* pArray) { return T(mxGetPr(pArray)[M]); }
}
