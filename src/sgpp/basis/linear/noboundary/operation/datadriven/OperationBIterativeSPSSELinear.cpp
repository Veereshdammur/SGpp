/******************************************************************************
* Copyright (C) 2010 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
******************************************************************************/
// @author Alexander Heinecke (Alexander.Heinecke@mytum.de)

#include "sgpp.hpp"
#include "basis/linear/noboundary/operation/datadriven/OperationBIterativeSPSSELinear.hpp"
#include "exception/operation_exception.hpp"

#ifdef USEOMP
#include "omp.h"
#endif

#ifdef USEICCINTRINSICS
// include SSE3 intrinsics
#include <pmmintrin.h>

union floatAbsMask
{
   const float f;
   const int i;

   floatAbsMask() : i(0x7FFFFFFF) {}
};

_MM_ALIGN16 const floatAbsMask absMask;
static const __m128 abs2Mask = _mm_load1_ps( &absMask.f );

const __m128 _mm_abs_ps( const __m128& x)
{
       return _mm_and_ps( abs2Mask, x);
}
#endif

#define CHUNKDATAPOINTS 24 // must be divide-able by 24
#define CHUNKGRIDPOINTS 12

namespace sg
{

OperationBIterativeSPSSELinear::OperationBIterativeSPSSELinear(GridStorage* storage) : storage(storage)
{
	Level = new DataMatrixSP(storage->size(), storage->dim());
	Index = new DataMatrixSP(storage->size(), storage->dim());

	storage->getLevelIndexArraysForEval(*Level, *Index);
}

OperationBIterativeSPSSELinear::~OperationBIterativeSPSSELinear()
{
	delete Level;
	delete Index;
}

void OperationBIterativeSPSSELinear::rebuildLevelAndIndex()
{
	delete Level;
	delete Index;

	Level = new DataMatrixSP(storage->size(), storage->dim());
	Index = new DataMatrixSP(storage->size(), storage->dim());

	storage->getLevelIndexArraysForEval(*Level, *Index);
}

void OperationBIterativeSPSSELinear::multVectorized(DataVectorSP& alpha, DataMatrixSP& data, DataVectorSP& result)
{
	size_t source_size = alpha.getSize();
    size_t dims = storage->dim();
    size_t storageSize = storage->size();
    float* ptrSource = alpha.getPointer();
    float* ptrData = data.getPointer();
    float* ptrLevel = this->Level->getPointer();
    float* ptrIndex = this->Index->getPointer();

    if (data.getNcols() % 4 != 0 || source_size != data.getNcols())
    {
    	throw operation_exception("For iterative mult transpose an even number of instances is required and result vector length must fit to data!");
    	return;
    }

    result.setAll(0.0);

#ifdef USEOMP
    #pragma omp parallel
	{
		size_t chunksize = (source_size/omp_get_num_threads())+1;
		// assure that every subarray is 16-byte aligned
		if (chunksize % 4 != 0)
		{
			size_t remainder = chunksize % 4;
			size_t patch = 4 - remainder;
			chunksize += patch;
		}
    	size_t start = chunksize*omp_get_thread_num();
    	size_t end = std::min<size_t>(start+chunksize, source_size);

    	DataVectorSP myResult(result.getSize());
    	myResult.setAll(0.0);
    	float* ptrResult = myResult.getPointer();
#else
    	size_t start = 0;
    	size_t end = source_size;
    	float* ptrResult = result.getPointer();
#endif
		for(size_t c = start; c < end; c+=std::min<size_t>((size_t)CHUNKDATAPOINTS, (end-c)))
		{
			size_t data_end = std::min<size_t>((size_t)CHUNKDATAPOINTS+c, end);

			for (size_t m = 0; m < storageSize; m+=std::min<size_t>((size_t)CHUNKGRIDPOINTS, (storageSize-m)))
			{
				size_t grid_end = std::min<size_t>((size_t)CHUNKGRIDPOINTS+m, storageSize);
#ifdef USEICCINTRINSICS
				if ((data_end-c) == CHUNKDATAPOINTS && (grid_end-m) == CHUNKGRIDPOINTS)
				{
					for (size_t i = c; i < c+CHUNKDATAPOINTS; i+=24)
					{
						for (size_t j = m; j < m+CHUNKGRIDPOINTS; j++)
						{
							__m128 support_0 = _mm_load_ps(&(ptrSource[i]));
							__m128 support_1 = _mm_load_ps(&(ptrSource[i+4]));
							__m128 support_2 = _mm_load_ps(&(ptrSource[i+8]));
							__m128 support_3 = _mm_load_ps(&(ptrSource[i+12]));
							__m128 support_4 = _mm_load_ps(&(ptrSource[i+16]));
							__m128 support_5 = _mm_load_ps(&(ptrSource[i+20]));

							__m128 one = _mm_set1_ps(1.0);
							__m128 zero = _mm_set1_ps(0.0);

							for (size_t d = 0; d < dims; d++)
							{
								__m128 eval_0 = _mm_load_ps(&(ptrData[(d*source_size)+i]));
								__m128 eval_1 = _mm_load_ps(&(ptrData[(d*source_size)+i+4]));
								__m128 eval_2 = _mm_load_ps(&(ptrData[(d*source_size)+i+8]));
								__m128 eval_3 = _mm_load_ps(&(ptrData[(d*source_size)+i+12]));
								__m128 eval_4 = _mm_load_ps(&(ptrData[(d*source_size)+i+16]));
								__m128 eval_5 = _mm_load_ps(&(ptrData[(d*source_size)+i+20]));

								__m128 level = _mm_load1_ps(&(ptrLevel[(j*dims)+d]));
								__m128 index = _mm_load1_ps(&(ptrIndex[(j*dims)+d]));

								eval_0 = _mm_mul_ps(eval_0, level);
								eval_1 = _mm_mul_ps(eval_1, level);
								eval_2 = _mm_mul_ps(eval_2, level);
								eval_3 = _mm_mul_ps(eval_3, level);
								eval_4 = _mm_mul_ps(eval_4, level);
								eval_5 = _mm_mul_ps(eval_5, level);

								eval_0 = _mm_sub_ps(eval_0, index);
								eval_1 = _mm_sub_ps(eval_1, index);
								eval_2 = _mm_sub_ps(eval_2, index);
								eval_3 = _mm_sub_ps(eval_3, index);
								eval_4 = _mm_sub_ps(eval_4, index);
								eval_5 = _mm_sub_ps(eval_5, index);

								eval_0 = _mm_abs_ps(eval_0);
								eval_1 = _mm_abs_ps(eval_1);
								eval_2 = _mm_abs_ps(eval_2);
								eval_3 = _mm_abs_ps(eval_3);
								eval_4 = _mm_abs_ps(eval_4);
								eval_5 = _mm_abs_ps(eval_5);

								eval_0 = _mm_sub_ps(one, eval_0);
								eval_1 = _mm_sub_ps(one, eval_1);
								eval_2 = _mm_sub_ps(one, eval_2);
								eval_3 = _mm_sub_ps(one, eval_3);
								eval_4 = _mm_sub_ps(one, eval_4);
								eval_5 = _mm_sub_ps(one, eval_5);

								eval_0 = _mm_max_ps(zero, eval_0);
								eval_1 = _mm_max_ps(zero, eval_1);
								eval_2 = _mm_max_ps(zero, eval_2);
								eval_3 = _mm_max_ps(zero, eval_3);
								eval_4 = _mm_max_ps(zero, eval_4);
								eval_5 = _mm_max_ps(zero, eval_5);

								support_0 = _mm_mul_ps(support_0, eval_0);
								support_1 = _mm_mul_ps(support_1, eval_1);
								support_2 = _mm_mul_ps(support_2, eval_2);
								support_3 = _mm_mul_ps(support_3, eval_3);
								support_4 = _mm_mul_ps(support_4, eval_4);
								support_5 = _mm_mul_ps(support_5, eval_5);
							}

							__m128 res_0 = _mm_setzero_ps();
							res_0 = _mm_load_ss(&(ptrResult[j]));

							support_0 = _mm_add_ps(support_0, support_1);
							support_2 = _mm_add_ps(support_2, support_3);
							support_4 = _mm_add_ps(support_4, support_5);
							support_0 = _mm_add_ps(support_0, support_2);
							support_0 = _mm_add_ps(support_0, support_4);

							support_0 = _mm_hadd_ps(support_0, support_0);
							support_0 = _mm_hadd_ps(support_0, support_0);
							res_0 = _mm_add_ss(res_0, support_0);

							_mm_store_ss(&(ptrResult[j]), res_0);
						}
					}
				}
				else
				{
					for (size_t i = c; i < data_end; i++)
					{
						for (size_t j = m; j < grid_end; j++)
						{
							float curSupport = ptrSource[i];

							#pragma ivdep
							#pragma vector aligned
							for (size_t d = 0; d < dims; d++)
							{
								float eval = ((ptrLevel[(j*dims)+d]) * (ptrData[(d*source_size)+i]));
								float index_calc = eval - (ptrIndex[(j*dims)+d]);
								float abs = fabs(index_calc);
								float last = 1.0 - abs;
								float localSupport = std::max<float>(last, 0.0);
								curSupport *= localSupport;
							}

							ptrResult[j] += curSupport;
						}
					}
				}
#else
				for (size_t i = c; i < data_end; i++)
				{
					for (size_t j = m; j < grid_end; j++)
					{
						float curSupport = ptrSource[i];
#ifdef __ICC
						#pragma ivdep
						#pragma vector aligned
#endif
						for (size_t d = 0; d < dims; d++)
						{
							float eval = ((ptrLevel[(j*dims)+d]) * (ptrData[(d*source_size)+i]));
							float index_calc = eval - (ptrIndex[(j*dims)+d]);
							float abs = fabs(index_calc);
							float last = 1.0 - abs;
							float localSupport = std::max<float>(last, 0.0);
							curSupport *= localSupport;
						}

						ptrResult[j] += curSupport;
					}
				}
#endif
	        }
		}
#ifdef USEOMP
		// sum private result vectors
		#pragma omp critical
		{
			result.add(myResult);
		}
	}
#endif
}

void OperationBIterativeSPSSELinear::multTransposeVectorized(DataVectorSP& alpha, DataMatrixSP& data, DataVectorSP& result)
{
	size_t result_size = result.getSize();
    size_t dims = storage->dim();
    size_t storageSize = storage->size();
    float* ptrAlpha = alpha.getPointer();
    float* ptrData = data.getPointer();
    float* ptrResult = result.getPointer();
    float* ptrLevel = this->Level->getPointer();
    float* ptrIndex = this->Index->getPointer();

    if (data.getNcols() % 4 != 0 || result_size != data.getNcols())
    {
    	throw operation_exception("For iterative mult transpose an even number of instances is required and result vector length must fit to data!");
    	return;
    }

#ifdef USEOMP
    #pragma omp parallel
	{
		size_t chunksize = (result_size/omp_get_num_threads())+1;
		// assure that every subarray is 16-byte aligned
		if (chunksize % 4 != 0)
		{
			size_t remainder = chunksize % 4;
			size_t patch = 4 - remainder;
			chunksize += patch;
		}
    	size_t start = chunksize*omp_get_thread_num();
    	size_t end = std::min<size_t>(start+chunksize, result_size);
#else
    	size_t start = 0;
    	size_t end = result_size;
#endif
		for(size_t c = start; c < end; c+=std::min<size_t>((size_t)CHUNKDATAPOINTS, (end-c)))
		{
			size_t data_end = std::min<size_t>((size_t)CHUNKDATAPOINTS+c, end);

#ifdef __ICC
			#pragma ivdep
			#pragma vector aligned
#endif
			for (size_t i = c; i < data_end; i++)
			{
				ptrResult[i] = 0.0;
			}

			for (size_t m = 0; m < storageSize; m+=std::min<size_t>((size_t)CHUNKGRIDPOINTS, (storageSize-m)))
			{
				size_t grid_end = std::min<size_t>((size_t)CHUNKGRIDPOINTS+m, storageSize);
#ifdef USEICCINTRINSICS
				if ((data_end-c) == CHUNKDATAPOINTS && (grid_end-m) == CHUNKGRIDPOINTS)
				{
					for (size_t i = c; i < c+CHUNKDATAPOINTS; i+=24)
					{
						for (size_t j = m; j < m+CHUNKGRIDPOINTS; j++)
						{
							__m128 support_0 = _mm_load1_ps(&(ptrAlpha[j]));
							__m128 support_1 = _mm_load1_ps(&(ptrAlpha[j]));
							__m128 support_2 = _mm_load1_ps(&(ptrAlpha[j]));
							__m128 support_3 = _mm_load1_ps(&(ptrAlpha[j]));
							__m128 support_4 = _mm_load1_ps(&(ptrAlpha[j]));
							__m128 support_5 = _mm_load1_ps(&(ptrAlpha[j]));

							__m128 one = _mm_set1_ps(1.0);
							__m128 zero = _mm_set1_ps(0.0);

							for (size_t d = 0; d < dims; d++)
							{
								__m128 eval_0 = _mm_load_ps(&(ptrData[(d*result_size)+i]));
								__m128 eval_1 = _mm_load_ps(&(ptrData[(d*result_size)+i+4]));
								__m128 eval_2 = _mm_load_ps(&(ptrData[(d*result_size)+i+8]));
								__m128 eval_3 = _mm_load_ps(&(ptrData[(d*result_size)+i+12]));
								__m128 eval_4 = _mm_load_ps(&(ptrData[(d*result_size)+i+16]));
								__m128 eval_5 = _mm_load_ps(&(ptrData[(d*result_size)+i+20]));

								__m128 level = _mm_load1_ps(&(ptrLevel[(j*dims)+d]));
								__m128 index = _mm_load1_ps(&(ptrIndex[(j*dims)+d]));

								eval_0 = _mm_mul_ps(eval_0, level);
								eval_1 = _mm_mul_ps(eval_1, level);
								eval_2 = _mm_mul_ps(eval_2, level);
								eval_3 = _mm_mul_ps(eval_3, level);
								eval_4 = _mm_mul_ps(eval_4, level);
								eval_5 = _mm_mul_ps(eval_5, level);

								eval_0 = _mm_sub_ps(eval_0, index);
								eval_1 = _mm_sub_ps(eval_1, index);
								eval_2 = _mm_sub_ps(eval_2, index);
								eval_3 = _mm_sub_ps(eval_3, index);
								eval_4 = _mm_sub_ps(eval_4, index);
								eval_5 = _mm_sub_ps(eval_5, index);

								eval_0 = _mm_abs_ps(eval_0);
								eval_1 = _mm_abs_ps(eval_1);
								eval_2 = _mm_abs_ps(eval_2);
								eval_3 = _mm_abs_ps(eval_3);
								eval_4 = _mm_abs_ps(eval_4);
								eval_5 = _mm_abs_ps(eval_5);

								eval_0 = _mm_sub_ps(one, eval_0);
								eval_1 = _mm_sub_ps(one, eval_1);
								eval_2 = _mm_sub_ps(one, eval_2);
								eval_3 = _mm_sub_ps(one, eval_3);
								eval_4 = _mm_sub_ps(one, eval_4);
								eval_5 = _mm_sub_ps(one, eval_5);

								eval_0 = _mm_max_ps(zero, eval_0);
								eval_1 = _mm_max_ps(zero, eval_1);
								eval_2 = _mm_max_ps(zero, eval_2);
								eval_3 = _mm_max_ps(zero, eval_3);
								eval_4 = _mm_max_ps(zero, eval_4);
								eval_5 = _mm_max_ps(zero, eval_5);

								support_0 = _mm_mul_ps(support_0, eval_0);
								support_1 = _mm_mul_ps(support_1, eval_1);
								support_2 = _mm_mul_ps(support_2, eval_2);
								support_3 = _mm_mul_ps(support_3, eval_3);
								support_4 = _mm_mul_ps(support_4, eval_4);
								support_5 = _mm_mul_ps(support_5, eval_5);
							}

							__m128 res_0 = _mm_load_ps(&(ptrResult[i]));
							__m128 res_1 = _mm_load_ps(&(ptrResult[i+4]));
							__m128 res_2 = _mm_load_ps(&(ptrResult[i+8]));
							__m128 res_3 = _mm_load_ps(&(ptrResult[i+12]));
							__m128 res_4 = _mm_load_ps(&(ptrResult[i+16]));
							__m128 res_5 = _mm_load_ps(&(ptrResult[i+20]));

							res_0 = _mm_add_ps(res_0, support_0);
							res_1 = _mm_add_ps(res_1, support_1);
							res_2 = _mm_add_ps(res_2, support_2);
							res_3 = _mm_add_ps(res_3, support_3);
							res_4 = _mm_add_ps(res_4, support_4);
							res_5 = _mm_add_ps(res_5, support_5);

							_mm_store_ps(&(ptrResult[i]), res_0);
							_mm_store_ps(&(ptrResult[i+4]), res_1);
							_mm_store_ps(&(ptrResult[i+8]), res_2);
							_mm_store_ps(&(ptrResult[i+12]), res_3);
							_mm_store_ps(&(ptrResult[i+16]), res_4);
							_mm_store_ps(&(ptrResult[i+20]), res_5);
						}
					}
				}
				else
				{
					for (size_t i = c; i < data_end; i++)
					{
						for (size_t j = m; j < grid_end; j++)
						{
							float curSupport = ptrAlpha[j];

							#pragma ivdep
							#pragma vector aligned
							for (size_t d = 0; d < dims; d++)
							{
								float eval = ((ptrLevel[(j*dims)+d]) * (ptrData[(d*result_size)+i]));
								float index_calc = eval - (ptrIndex[(j*dims)+d]);
								float abs = fabs(index_calc);
								float last = 1.0 - abs;
								float localSupport = std::max<float>(last, 0.0);
								curSupport *= localSupport;
							}

							ptrResult[i] += curSupport;
						}
					}
				}
#else
				for (size_t i = c; i < data_end; i++)
				{
					for (size_t j = m; j < grid_end; j++)
					{
						float curSupport = ptrAlpha[j];
#ifdef __ICC
						#pragma ivdep
						#pragma vector aligned
#endif
						for (size_t d = 0; d < dims; d++)
						{
							float eval = ((ptrLevel[(j*dims)+d]) * (ptrData[(d*result_size)+i]));
							float index_calc = eval - (ptrIndex[(j*dims)+d]);
							float abs = fabs(index_calc);
							float last = 1.0 - abs;
							float localSupport = std::max<float>(last, 0.0);
							curSupport *= localSupport;
						}

						ptrResult[i] += curSupport;
					}
				}
#endif
	        }
		}
#ifdef USEOMP
	}
#endif
}

}
