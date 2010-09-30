/******************************************************************************
* Copyright (C) 2010 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
******************************************************************************/
// @author Alexander Heinecke (Alexander.Heinecke@mytum.de)

#include "sgpp.hpp"
#include "basis/linear/noboundary/operation/datadriven/OperationBIterativeSPOCLLinear.hpp"
#include "exception/operation_exception.hpp"

#ifdef USEOMP
#include "omp.h"
#endif

namespace sg
{

OperationBIterativeSPOCLLinear::OperationBIterativeSPOCLLinear(GridStorage* storage) : storage(storage)
{
	Level = new DataMatrixSP(storage->size(), storage->dim());
	Index = new DataMatrixSP(storage->size(), storage->dim());

	storage->getLevelIndexArraysForEval(*Level, *Index);

	myTimer = new SGppStopwatch();
	myOCLKernels = new OCLKernels();
}

OperationBIterativeSPOCLLinear::~OperationBIterativeSPOCLLinear()
{
	delete Level;
	delete Index;
	delete myTimer;
	delete myOCLKernels;
}

void OperationBIterativeSPOCLLinear::rebuildLevelAndIndex()
{
	delete Level;
	delete Index;

	Level = new DataMatrixSP(storage->size(), storage->dim());
	Index = new DataMatrixSP(storage->size(), storage->dim());

	storage->getLevelIndexArraysForEval(*Level, *Index);

	myOCLKernels->resetKernels();
}

double OperationBIterativeSPOCLLinear::multVectorized(DataVectorSP& alpha, DataMatrixSP& data, DataVectorSP& result)
{
	size_t source_size = alpha.getSize();
    size_t dims = storage->dim();
    size_t storageSize = storage->size();

    result.setAll(0.0f);

    float* ptrSource = alpha.getPointer();
    float* ptrData = data.getPointer();
    float* ptrLevel = this->Level->getPointer();
    float* ptrIndex = this->Index->getPointer();
    float* ptrGlobalResult = result.getPointer();

    if (data.getNrows() % 64 != 0 || source_size != data.getNrows())
    {
    	throw operation_exception("For iterative mult an even number of instances is required and result vector length must fit to data!");
    }

    double time = myOCLKernels->multSPOCL(ptrSource, ptrData, ptrLevel, ptrIndex, ptrGlobalResult, source_size, storageSize, dims);

	return time;
}

double OperationBIterativeSPOCLLinear::multTransposeVectorized(DataVectorSP& alpha, DataMatrixSP& data, DataVectorSP& result)
{
	size_t result_size = result.getSize();
    size_t dims = storage->dim();
    size_t storageSize = storage->size();

    result.setAll(0.0f);

    float* ptrAlpha = alpha.getPointer();
    float* ptrData = data.getPointer();
    float* ptrResult = result.getPointer();
    float* ptrLevel = this->Level->getPointer();
    float* ptrIndex = this->Index->getPointer();

    if (data.getNrows() % 64 != 0 || result_size != data.getNrows())
    {
    	throw operation_exception("For iterative mult transpose an even number of instances is required and result vector length must fit to data!");
    }

    double time = myOCLKernels->multTransSPOCL(ptrAlpha, ptrData, ptrLevel, ptrIndex, ptrResult, result_size, storageSize, dims);

   	return time;
}

}
