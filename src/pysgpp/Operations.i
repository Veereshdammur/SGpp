/******************************************************************************
* Copyright (C) 2009 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
******************************************************************************/
// @author Dirk Pflueger (pflueged@in.tum.de), Joerg Blank (blankj@in.tum.de), Alexander Heinecke (Alexander.Heinecke@mytum.de)

namespace sg
{
//-     namespace base ------------------------------------------------
namespace base
{

class RefinementFunctor
{
public:
	typedef double value_type;

	virtual double operator()(GridStorage* storage, size_t seq) = 0;
	virtual double start() = 0;	
};

class CoarseningFunctor
{
public:
	typedef double value_type;

	virtual double operator()(GridStorage* storage, size_t seq) = 0;
	virtual double start() = 0;	
};

class GridGenerator
{
public:
	virtual void regular(size_t level) = 0;
	virtual void truncated(size_t level,size_t l_user) = 0;
	virtual void refine(RefinementFunctor* func) = 0;
	virtual void coarsen(CoarseningFunctor* func, DataVector* alpha) = 0;
	virtual void coarsenNFirstOnly(CoarseningFunctor* func, DataVector* alpha, size_t numFirstOnly) = 0;
	virtual int getNumberOfRefinablePoints() = 0;
	virtual int getNumberOfRemoveablePoints() = 0;
	virtual void refineMaxLevel(RefinementFunctor* func, unsigned int maxLevel) = 0;
	virtual int getNumberOfRefinablePointsToMaxLevel(unsigned int maxLevel) = 0;
};

class OperationMultipleEval
{
public:
	virtual void mult(DataVector& alpha, DataVector& result) = 0;
	virtual void multTranspose(DataVector& soruce, DataVector& result) = 0;
};

class OperationMultipleEvalVectorized
{
public:
	virtual double multVectorized(DataVector& alpha, DataVector& result) = 0;
	virtual double multTransposeVectorized(DataVector& source, DataVector& result) = 0;
	virtual void rebuildLevelAndIndex() = 0;
};

class OperationMatrix
{
public:
	virtual void mult(DataVector& alpha, DataVector& result) = 0;
};

class OperationConvert
{
public:
	virtual void doConvertToLinear(DataVector& alpha) = 0;
	virtual void doConvertFromLinear(DataVector& alpha) = 0;
};

class OperationEval
{
public:
	virtual double eval(DataVector& alpha, DataVector& point) = 0;
};

class OperationHierarchisation
{
public:
	virtual void doHierarchisation(DataVector& node_values) = 0;
	virtual void doDehierarchisation(DataVector& alpha) = 0;
};
}

namespace base {
class OperationQuadrature
{
public:
	virtual double doQuadrature(DataVector& alpha) = 0;
};

}
//- end namespace base ------------------------------------------------

//-     namespace datadriven ------------------------------------------
namespace datadriven {
%nodefaultdtor sg::datadriven::OperationTest;
class OperationTest
{
public:
  virtual double test(base::DataVector& alpha, base::DataMatrix& data, base::DataVector& classes) = 0;
	virtual double testMSE(base::DataVector& alpha, base::DataMatrix& data, base::DataVector& refValues) = 0;
	virtual double testWithCharacteristicNumber(base::DataVector& alpha, base::DataMatrix& data, base::DataVector& classes, base::DataVector& charaNumbers) = 0;
};
}
//- end namespace datadriven ------------------------------------------

}

